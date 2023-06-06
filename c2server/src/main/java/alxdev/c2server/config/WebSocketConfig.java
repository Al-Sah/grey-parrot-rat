package alxdev.c2server.config;


import alxdev.c2server.ws.bot.AgentHandshakeInterceptor;
import alxdev.c2server.ws.bot.AgentWebSocketHandler;
import alxdev.c2server.ws.operator.OperatorHandshakeInterceptor;
import alxdev.c2server.ws.operator.OperatorWebSocketHandler;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;

@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer{

    private final AgentWebSocketHandler agentWebSocketHandler;
    private final AgentHandshakeInterceptor agentHandshakeInterceptor;

    private final OperatorWebSocketHandler operatorWebSocketHandler;
    private final OperatorHandshakeInterceptor operatorHandshakeInterceptor;


    public WebSocketConfig(AgentWebSocketHandler agentWebSocketHandler, AgentHandshakeInterceptor agentHandshakeInterceptor, OperatorWebSocketHandler operatorWebSocketHandler, OperatorHandshakeInterceptor operatorHandshakeInterceptor) {

        this.agentWebSocketHandler = agentWebSocketHandler;
        this.agentHandshakeInterceptor = agentHandshakeInterceptor;
        this.operatorWebSocketHandler = operatorWebSocketHandler;
        this.operatorHandshakeInterceptor = operatorHandshakeInterceptor;
    }

    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        registry.addHandler(agentWebSocketHandler, "/agent/*")
                .addInterceptors(agentHandshakeInterceptor)
                .setAllowedOrigins("*")

                .addHandler(operatorWebSocketHandler, "/operator/*")
                .addInterceptors(operatorHandshakeInterceptor)
                .setAllowedOrigins("*");
    }
}
