package alxdev.c2server.ws.bot;

import lombok.extern.slf4j.Slf4j;
import org.springframework.lang.NonNull;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.lang.Nullable;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.HandshakeInterceptor;

import java.net.URI;
import java.util.Map;

@Slf4j
@Component
public class AgentHandshakeInterceptor implements HandshakeInterceptor {


    private static final String PATH_PREFIX = "/agent/";
    private static final int MIN_BOT_ID_LENGTH = 8;

    @Override
    public boolean beforeHandshake(
            @NonNull ServerHttpRequest request,
            @NonNull ServerHttpResponse response,
            @NonNull WebSocketHandler wsHandler,
            @NonNull Map<String, Object> attributes) {

        // First step: retrieve agent id
        String agentId = extractId(request.getURI());
        if(agentId.length() < MIN_BOT_ID_LENGTH){
            log.warn("failed to identify agent; received id length is {}", agentId.length());
            return false;
        }

        // TODO add custom auth ?

        attributes.put("agent-id", agentId);
        return true;
    }

    @Override
    public void afterHandshake(
            @NonNull ServerHttpRequest request,
            @NonNull ServerHttpResponse response,
            @NonNull WebSocketHandler wsHandler,
            @Nullable Exception exception) { }


    private String extractId(URI uri){
        try {
            return uri.getPath().substring(PATH_PREFIX.length());
        } catch (RuntimeException e){
            log.warn(e.toString());
            return "";
        }
    }
}
