package alxdev.c2server.services.impl;

import alxdev.c2server.models.Agent;
import alxdev.c2server.models.AgentInfo;
import alxdev.c2server.services.ClientsSessionsHolder;
import org.springframework.stereotype.Service;
import org.springframework.web.socket.WebSocketSession;

import lombok.extern.slf4j.Slf4j;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Slf4j
@Service
public class ClientsSessionsHolderBase implements ClientsSessionsHolder {


    Map<String, Agent> activeAgents = new HashMap<>();
    Map<String, WebSocketSession> activeOperators = new HashMap<>();


    @Override
    public void addAgent(WebSocketSession session, String id) {
        var agentInfo = new AgentInfo();
        agentInfo.setState(AgentInfo.InfoState.COLLECTING);

        activeAgents.put(id, new Agent(agentInfo,session));
        log.debug("Added new agent: id - '{}', session - '{}'", id, session);
    }

    @Override
    public void removeAgent(String id) {
        try {
            activeAgents.remove(id);
        }catch (RuntimeException e){
            log.warn("Failed to remove agent {}", id);
        }
    }


    @Override
    public Agent getAgent(String id) {
        return activeAgents.get(id);
    }

    @Override
    public List<Agent> getAgents() {
        return activeAgents.values().stream().toList();
    }

    @Override
    public List<WebSocketSession> getOperators() {
        return activeOperators.values().stream().toList();
    }

    @Override
    public void addOperator(WebSocketSession session, String id) {
        activeOperators.put(id, session);
        log.debug("Added new operator: id - '{}', session - '{}'", id, session);
    }

    @Override
    public void removeOperator(String id) {
        try {
            activeOperators.remove(id);
        }catch (RuntimeException e){
            log.debug("Failed to remove operator {}", id);
        }
    }

    @Override
    public WebSocketSession getOperator(String id) {
        return activeOperators.get(id);
    }
}
