package alxdev.c2server.config;


import alxdev.c2server.ws.bot.BotHandshakeInterceptor;
import alxdev.c2server.ws.bot.BotWebSocketHandler;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;

@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer{

    private final BotWebSocketHandler botWebSocketHandler;
    private final BotHandshakeInterceptor botHandshakeInterceptor;


    public WebSocketConfig(BotWebSocketHandler botWebSocketHandler, BotHandshakeInterceptor botHandshakeInterceptor) {

        this.botWebSocketHandler = botWebSocketHandler;
        this.botHandshakeInterceptor = botHandshakeInterceptor;
    }

    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        registry.addHandler(botWebSocketHandler, "/bot/*")
                .addInterceptors(botHandshakeInterceptor)
                .setAllowedOrigins("*");
    }
}
