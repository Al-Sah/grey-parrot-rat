package alxdev.c2server.ws.bot;

import alxdev.c2server.models.BotControlMessageHeader;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.*;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;

import java.util.Arrays;

@Slf4j
@Component
public class BotWebSocketHandler extends AbstractWebSocketHandler {

    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {

        log.info("bot {} connected", session.getAttributes().get("bot-id"));
        var header = new BotControlMessageHeader(1, "test-module", (short) 6, 1000, true).serialize();

        // tmp sample
        byte[] payload = "some payload".getBytes();
        byte[] message = Arrays.copyOf(header, header.length + payload.length);
        System.arraycopy(payload, 0, message, header.length, payload.length);
        session.sendMessage(new BinaryMessage( message ));
    }

    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        log.info("received text message from bot {}", session.getAttributes().get("bot-id"));
    }

    @Override
    protected void handleBinaryMessage(WebSocketSession session, BinaryMessage message) throws Exception {
        log.info("received binary message from bot {}", session.getAttributes().get("bot-id"));
    }

    @Override
    public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
        log.error("session {}: transport error {}", session.getAttributes().get("bot-id"), exception.getMessage());
    }

    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        log.info("bot {} disconnected with status {}", session.getAttributes().get("bot-id"), status.toString());
    }
}