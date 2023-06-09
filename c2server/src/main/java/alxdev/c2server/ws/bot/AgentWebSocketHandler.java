package alxdev.c2server.ws.bot;

import alxdev.c2server.models.AgentInfo;
import alxdev.c2server.models.proto.Control;
import alxdev.c2server.models.proto.CoreData;
import alxdev.c2server.services.ClientsSessionsHolder;
import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;
import lombok.extern.slf4j.Slf4j;
import org.springframework.lang.NonNull;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.*;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

import java.io.IOException;
import java.nio.charset.StandardCharsets;

@Slf4j
@Component
public class AgentWebSocketHandler extends AbstractWebSocketHandler {

    private final ClientsSessionsHolder clientsSessionsHolder;

    public AgentWebSocketHandler(ClientsSessionsHolder clientsSessionsHolder) {
        this.clientsSessionsHolder = clientsSessionsHolder;
    }

    @Override
    public void afterConnectionEstablished(@NonNull WebSocketSession session) throws Exception {

        String agentId = getAgentId(session);
        log.info("connected new agent {}", agentId);

        clientsSessionsHolder.addAgent(session, agentId);

        var header = Control.ControlHeader.newBuilder()
                .setType(Control.ControlHeader.MessageType.SINGLE)
                .setRequestId(1)
                .setModule("agent-control")
                .setIsClosing(true)
                .build();

        var controlPacket = Control.ControlPacket.newBuilder()
                .setHeader(header)
                .setPayload(ByteString.copyFrom("get-state", StandardCharsets.US_ASCII))
                .build();

        var message = Control.NetworkMessage.newBuilder()
                .setControl(controlPacket)
                .build();

        session.sendMessage(new BinaryMessage( message.toByteArray() ));
    }

    @Override
    protected void handleTextMessage(@NonNull WebSocketSession session, @NonNull TextMessage message) {
        log.warn("received text message from agent {}; remote address {}", getAgentId(session), session.getRemoteAddress());
    }

    @Override
    protected void handleBinaryMessage(@NonNull WebSocketSession session, @NonNull BinaryMessage message) {

        String agentId = getAgentId(session);
        log.info("received binary message from agent {}", agentId);

        Control.NetworkMessage networkMessage;
        try{
            networkMessage = Control.NetworkMessage.parseFrom(message.getPayload().array());
        }catch (InvalidProtocolBufferException e) {
            log.warn("agent {}: failed to parse inbox message, error: {}", agentId, e.getMessage());
            return;
        }

        switch (networkMessage.getDataCase()) {
            case CONTROL -> handleControlMessage(networkMessage.getControl(), agentId);
            case SIGNALING -> handleSignalingMessage(networkMessage.getSignaling(), agentId);
            case DATA_NOT_SET -> log.warn("agent {}: inbox message does not contain data", agentId);
        }
    }

    @Override
    public void handleTransportError(@NonNull WebSocketSession session, @NonNull Throwable exception) {
        log.error("agent {}: transport error {}", getAgentId(session), exception.getMessage());
    }

    @Override
    public void afterConnectionClosed(@NonNull WebSocketSession session, CloseStatus status) throws Exception {
        String agentId = getAgentId(session);
        log.info("agent {} disconnected with status {}", agentId, status.toString());

        clientsSessionsHolder.removeAgent(agentId);
        var outMessage = generateNotification(agentId);

        for (WebSocketSession operator : clientsSessionsHolder.getOperators()) {
            operator.sendMessage(new BinaryMessage( outMessage.toByteArray() ));
        }
    }

    void handleControlMessage(Control.ControlPacket control, String agentId){
        try{
            if(control.getHeader().getType() != Control.ControlHeader.MessageType.SINGLE){
                log.warn("Received partial message; not implemented");
                return;
            }
            // handling core data
            if(control.getHeader().getRequestId() == 1){
                var agent = clientsSessionsHolder.getAgent(agentId);
                var agentDescription = CoreData.AgentDescription.parseFrom(control.getPayload());
                var isNew = agent.getInfo().getState() == AgentInfo.InfoState.COLLECTING;
                agent.getInfo().setState(AgentInfo.InfoState.VALID);
                agent.getInfo().setDescription(agentDescription);

                var outMessage = generateNotification(isNew, agentDescription);

                for (WebSocketSession operator : clientsSessionsHolder.getOperators()) {
                    operator.sendMessage(new BinaryMessage( outMessage.toByteArray() ));
                }
            }
        }catch (IOException e) {
            e.printStackTrace();
        }
    }
    void handleSignalingMessage(Control.SignalingData signalingData, String agentId){

        WebSocketSession operator = clientsSessionsHolder.getOperator(signalingData.getId());
        if (operator == null){
            log.warn("handleSignalingMessage; operator is null");
            return;
        }

        var message = Control.NetworkMessage.newBuilder()
                .setSignaling(signalingData.toBuilder().setId(agentId).build())
                .build();

        try {
            operator.sendMessage(new BinaryMessage( message.toByteArray() ));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }


    private String getAgentId(WebSocketSession session){
        return (String) session.getAttributes().get("agent-id");
    }


    Control.NetworkMessage generateNotification(boolean isNew, CoreData.AgentDescription agentDescription){
        var header = Control.ControlHeader.newBuilder()
                .setType(Control.ControlHeader.MessageType.SINGLE)
                .setRequestId(1)
                .setModule("system-state")
                .setIsClosing(false)
                .build();

        var ctrlPayload = CoreData.CtrlModuleMessage.newBuilder()
                .setType(isNew ? CoreData.CtrlModuleMessage.Type.NEW : CoreData.CtrlModuleMessage.Type.UPDATE)
                .setAgent(agentDescription)
                .build();

        var resultControlPacket = Control.ControlPacket.newBuilder()
                .setHeader(header)
                .setPayload(ByteString.copyFrom(ctrlPayload.toByteArray()))
                .build();

        return Control.NetworkMessage.newBuilder()
                .setControl(resultControlPacket)
                .build();
    }

    Control.NetworkMessage generateNotification(String moduleId){
        var header = Control.ControlHeader.newBuilder()
                .setType(Control.ControlHeader.MessageType.SINGLE)
                .setRequestId(1)
                .setModule("system-state")
                .setIsClosing(false)
                .build();

        var ctrlPayload = CoreData.CtrlModuleMessage.newBuilder()
                .setType(CoreData.CtrlModuleMessage.Type.DISCONNECT)
                .setAgentId(moduleId)
                .build();

        var resultControlPacket = Control.ControlPacket.newBuilder()
                .setHeader(header)
                .setPayload(ByteString.copyFrom(ctrlPayload.toByteArray()))
                .build();

        return Control.NetworkMessage.newBuilder()
                .setControl(resultControlPacket)
                .build();
    }
}