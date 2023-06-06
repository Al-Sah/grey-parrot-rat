package alxdev.c2server.models;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;
import org.springframework.web.socket.WebSocketSession;

@Getter
@Setter
@ToString
@AllArgsConstructor
public class Agent {

    private AgentInfo info;
    private final WebSocketSession session;
}
