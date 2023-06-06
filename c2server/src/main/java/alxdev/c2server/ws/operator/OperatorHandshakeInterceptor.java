package alxdev.c2server.ws.operator;

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
public class OperatorHandshakeInterceptor implements HandshakeInterceptor {


    private static final String PATH_PREFIX = "/operator/";
    private static final int MIN_OPERATOR_ID_LENGTH = 8;

    @Override
    public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Map<String, Object> attributes) throws Exception {

        // First step: retrieve bot id
        String botID = extractId(request.getURI());
        if(botID.length() < MIN_OPERATOR_ID_LENGTH){
            log.info("Failed to identify operator; received id length is {}", botID.length());
            return false;
        }

        // TODO add custom auth ?

        attributes.put("operator-id", botID);
        return true;
    }

    @Override
    public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler, Exception exception) { }



    private String extractId(URI uri){
        try {
            return uri.getPath().substring(PATH_PREFIX.length());
        } catch (RuntimeException e){
            log.warn(e.toString());
            return "";
        }
    }
}
