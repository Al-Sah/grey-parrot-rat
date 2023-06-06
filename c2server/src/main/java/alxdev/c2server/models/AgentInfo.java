package alxdev.c2server.models;


import alxdev.c2server.models.proto.CoreData;
import lombok.Getter;
import lombok.Setter;
@Getter
@Setter
public class AgentInfo {

    public enum InfoState{
        COLLECTING,
        VALID
    }

    private InfoState state;
    private CoreData.AgentDescription description;
}
