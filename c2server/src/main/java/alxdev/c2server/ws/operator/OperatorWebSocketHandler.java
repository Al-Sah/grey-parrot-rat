package alxdev.c2server.ws.operator;

import alxdev.c2server.models.Agent;
import alxdev.c2server.models.AgentInfo;
import alxdev.c2server.models.proto.Control;
import alxdev.c2server.models.proto.CoreData;
import alxdev.c2server.services.ClientsSessionsHolder;
import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;
import lombok.extern.slf4j.Slf4j;
import org.springframework.lang.NonNull;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

import java.io.IOException;


@Slf4j
@Component
public class OperatorWebSocketHandler extends AbstractWebSocketHandler {

    private final ClientsSessionsHolder clientsSessionsHolder;

    public OperatorWebSocketHandler(ClientsSessionsHolder clientsSessionsHolder) {
        this.clientsSessionsHolder = clientsSessionsHolder;
    }

    @Override
    public void afterConnectionEstablished(@NonNull WebSocketSession session) throws Exception {
        String operatorId = getOperatorId(session);
        log.info("connected new operator {}", operatorId);

        clientsSessionsHolder.addOperator(session, operatorId);
        // TODO check whether operator was added successfully

        var agentsBuilder = CoreData.ActiveAgents.newBuilder();

        for (Agent agent : clientsSessionsHolder.getAgents()) {
            if(agent.getInfo().getState() == AgentInfo.InfoState.COLLECTING){
                continue;
            }
            agentsBuilder.addData(agent.getInfo().getDescription());
        }

        session.sendMessage(new BinaryMessage(generateNotification(agentsBuilder.build()).toByteArray()));
    }

    @Override
    protected void handleTextMessage(@NonNull WebSocketSession session, @NonNull TextMessage message) {
        log.info("received text message from bot {}", getOperatorId(session));
    }

    @Override
    protected void handleBinaryMessage( @NonNull  WebSocketSession session, @NonNull BinaryMessage message) {
        String operatorId = getOperatorId(session);
        log.info("received binary message from operator {}", operatorId);

        Control.NetworkMessage networkMessage;
        try{
            networkMessage = Control.NetworkMessage.parseFrom(message.getPayload().array());
        }catch (InvalidProtocolBufferException e) {
            log.warn("agent {}: failed to parse inbox message, error: {}", operatorId, e.getMessage());
            return;
        }

        switch (networkMessage.getDataCase()) {
            case CONTROL -> handleControlMessage(networkMessage.getControl(), operatorId);
            case SIGNALING -> handleSignalingMessage(networkMessage.getSignaling(), operatorId);
            case DATA_NOT_SET -> log.warn("operator {}: inbox message does not contain data", operatorId);
        }
    }

    @Override
    public void handleTransportError( @NonNull WebSocketSession session, @NonNull Throwable exception) {
        log.error("session {}: transport error {}", getOperatorId(session), exception.getMessage());
    }

    @Override
    public void afterConnectionClosed( @NonNull WebSocketSession session, CloseStatus status) {
        String operatorId = getOperatorId(session);
        log.info("operator {} disconnected with status {}", operatorId, status.toString());
        clientsSessionsHolder.removeOperator(operatorId);
    }

    private String getOperatorId(WebSocketSession session){
        return (String) session.getAttributes().get("operator-id");
    }

    void handleControlMessage(Control.ControlPacket control, String agentId){

    }

    void handleSignalingMessage(Control.SignalingData signalingData, String operatorId){

        WebSocketSession agent = clientsSessionsHolder.getAgent(signalingData.getId()).getSession();
        if (agent == null){
            log.warn("handleSignalingMessage; agent is null");
            return;
        }

        var message = Control.NetworkMessage.newBuilder()
                .setSignaling(signalingData.toBuilder().setId(operatorId).build())
                .build();

        try {
            agent.sendMessage(new BinaryMessage( message.toByteArray() ));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    Control.NetworkMessage generateNotification(CoreData.ActiveAgents agents){
        var header = Control.ControlHeader.newBuilder()
                .setType(Control.ControlHeader.MessageType.SINGLE)
                .setRequestId(1)
                .setModule("system-state")
                .setIsClosing(false)
                .build();

        var ctrlPayload = CoreData.CtrlModuleMessage.newBuilder()
                .setType(CoreData.CtrlModuleMessage.Type.SET_ALL)
                .setAgents(agents)
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