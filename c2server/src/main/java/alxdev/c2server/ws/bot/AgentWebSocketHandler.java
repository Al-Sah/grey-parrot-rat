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

        log.info("bot {} connected", session.getAttributes().get("agent-id"));

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
        log.info("received text message from bot {}", session.getAttributes().get("agent-id"));
    }

    @Override
    protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {

        try{

            var agent = clientsSessionsHolder.getAgent((String)session.getAttributes().get("bot-id"));

            var controlPacket = Control.NetworkMessage.parseFrom(message.getPayload().array()).getControl();

            var agentDescription = CoreData.AgentDescription.parseFrom(controlPacket.getPayload());

            agent.getInfo().setState(AgentInfo.InfoState.VALID);
            agent.getInfo().setDescription(agentDescription);


        }catch (IOException e) {
            e.printStackTrace();
        }
        log.info("received binary message from bot {}", session.getAttributes().get("agent-id"));
    }

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
        log.error("session {}: transport error {}", session.getAttributes().get("agent-id"), exception.getMessage());
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        log.info("bot {} disconnected with status {}", session.getAttributes().get("agent-id"), status.toString());
    }
}