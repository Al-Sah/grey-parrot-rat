package alxdev.c2server.ws.bot;

import lombok.extern.slf4j.Slf4j;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.HandshakeInterceptor;

import java.net.URI;
import java.util.Map;

@Slf4j
@Component
public class BotHandshakeInterceptor implements HandshakeInterceptor {


    private static final String PATH_PREFIX = "/bot/";
    private static final int MIN_BOT_ID_LENGTH = 8;

    @Override
    public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Map<String, Object> attributes) throws Exception {

        // First step: retrieve bot id
        String botID = extractBotId(request.getURI());
        if(botID.length() < MIN_BOT_ID_LENGTH){
            log.info("Failed to identify bot; received id length is {}", botID.length());
            return false;
        }

        // TODO add custom auth ?

        attributes.put("bot-id", botID);
        return true;
    }

    @Override
    public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Exception exception) { }



    private String extractBotId(URI uri){
        try {
            return uri.getPath().substring(PATH_PREFIX.length());
        } catch (RuntimeException e){
            log.warn(e.toString());
            return "";
        }
    }
}
