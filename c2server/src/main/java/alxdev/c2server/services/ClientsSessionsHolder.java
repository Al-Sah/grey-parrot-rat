package alxdev.c2server.services;

import alxdev.c2server.models.Agent;
import org.springframework.web.socket.WebSocketSession;

import java.util.List;

public interface ClientsSessionsHolder {


    void addAgent(WebSocketSession session, String id);

    void removeAgent(String id);

    Agent getAgent(String id);


    List<Agent> getAgents(); // TODO: add filter


    List<WebSocketSession> getOperators(); // TODO: add filter

    void addOperator(WebSocketSession session, String id);

    void removeOperator(String id);

    WebSocketSession getOperator(String id);

}
