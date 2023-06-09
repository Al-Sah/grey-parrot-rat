package alxdev.c2server.ws.bot;

import alxdev.c2server.models.AgentInfo;
import alxdev.c2server.models.proto.Control;
import alxdev.c2server.models.proto.CoreData;
import alxdev.c2server.services.ClientsSessionsHolder;
import com.google.protobuf.ByteString;
import lombok.extern.slf4j.Slf4j;
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
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {

        clientsSessionsHolder.addAgent(session);

        log.info("bot {} connected", getAgentId(session));

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
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        log.warn("received text message from bot {}", getAgentId(session));
    }

    @Override
    protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {

        log.info("received binary message from bot {}", getAgentId(session));
        try{
            var controlPacket = Control.NetworkMessage.parseFrom(message.getPayload().array()).getControl();

            if(controlPacket.getHeader().getType() != Control.ControlHeader.MessageType.SINGLE){
                log.warn("Received partial message; not implemented");
                return;
            }

            // handling core data
            if(controlPacket.getHeader().getRequestId() == 1){
                var agent = clientsSessionsHolder.getAgent((String)session.getAttributes().get("bot-id"));
                var agentDescription = CoreData.AgentDescription.parseFrom(controlPacket.getPayload());
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

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
        log.error("session {}: transport error {}", getAgentId(session), exception.getMessage());
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        log.info("bot {} disconnected with status {}", getAgentId(session), status.toString());

        clientsSessionsHolder.removeAgent(getAgentId(session));
        var outMessage = generateNotification(getAgentId(session));

        for (WebSocketSession operator : clientsSessionsHolder.getOperators()) {
            operator.sendMessage(new BinaryMessage( outMessage.toByteArray() ));
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