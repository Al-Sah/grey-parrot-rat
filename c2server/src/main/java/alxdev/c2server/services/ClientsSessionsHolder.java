package alxdev.c2server.services;

import alxdev.c2server.models.Agent;
import org.springframework.web.socket.WebSocketSession;

import java.util.List;

public interface ClientsSessionsHolder {


    void addAgent(WebSocketSession session);

    void removeAgent(String id);

    Agent getAgent(String id);


    List<Agent> getAgents(); // TODO: add filter



    void addOperator(WebSocketSession session);

    void removeOperator(String id);

    WebSocketSession getOperator(String id);

}
