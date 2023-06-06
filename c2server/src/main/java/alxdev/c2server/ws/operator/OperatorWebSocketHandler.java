package alxdev.c2server.ws.operator;

import alxdev.c2server.models.Agent;
import alxdev.c2server.models.AgentInfo;
import alxdev.c2server.models.proto.Control;
import alxdev.c2server.models.proto.CoreData;
import alxdev.c2server.services.ClientsSessionsHolder;
import com.google.protobuf.ByteString;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;


@Slf4j
@Component
public class OperatorWebSocketHandler extends AbstractWebSocketHandler {

    private final ClientsSessionsHolder clientsSessionsHolder;

    public OperatorWebSocketHandler(ClientsSessionsHolder clientsSessionsHolder) {
        this.clientsSessionsHolder = clientsSessionsHolder;
    }

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {

        clientsSessionsHolder.addOperator(session);
        // TODO check whether operator was added successfully

        var agentsBuilder = CoreData.ActiveAgents.newBuilder();

        for (Agent agent : clientsSessionsHolder.getAgents()) {
            if(agent.getInfo().getState() == AgentInfo.InfoState.COLLECTING){
                continue;
            }
            agentsBuilder.addData(agent.getInfo().getDescription());
        }

        var header = Control.ControlHeader.newBuilder()
                .setType(Control.ControlHeader.MessageType.SINGLE)
                .setRequestId(1)
                .setModule("system-state")
                .setIsClosing(false)
                .build();

        var controlPacket = Control.ControlPacket.newBuilder()
                .setHeader(header)
                .setPayload(ByteString.copyFrom(agentsBuilder.build().toByteArray()))
                .build();

        var message = Control.NetworkMessage.newBuilder()
                .setControl(controlPacket)
                .build();


        session.sendMessage(new BinaryMessage(message.toByteArray()));
    }

    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        log.info("received text message from bot {}", session.getAttributes().get("operator-id"));
    }

    @Override
    protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {
        log.info("received binary message from bot {}", session.getAttributes().get("operator-id"));
    }

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
        log.error("session {}: transport error {}", session.getAttributes().get("operator-id"), exception.getMessage());
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        log.info("operator {} disconnected with status {}", session.getAttributes().get("operator-id"), status.toString());
    }
}