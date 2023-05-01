package alxdev.c2server.models;

import java.nio.ByteBuffer;

public class BotControlMessageHeader {

    public enum MessageType {
        SINGLE(0),  // 0000
        FIRST(1),   // 0001
        NEXT(2),    // 0010
        LAST(3),    // 0011
        FIRST_WITH_ACCUMULATION_FLAG(5); // 0(X)01 (In Binary) where X is an accumulate flag

        public final byte asByte;
        MessageType(int i) {
            asByte = (byte)i;
        }
    }

    // message header size in bytes (just main section)
    private final static short MAIN_SECTION_SIZE = 14;
    private final short headerSize;
    private final MessageType type;
    private final long request;
    private String module; // module identifier, must be present in 'FIRST' and 'SINGLE' msg types

    // 'FIRST' message additional section
    private short parts; // number of parts
    private int payloadSize; // full payload size


    // 'CONTINUATION' message:
    private short sequence;


    /**
     * 'SINGLE' message constructor
     * @param request unique request id
     * @param module module identifier. max 255 chars
     */
    public BotControlMessageHeader(long request, String module){
        this(MessageType.SINGLE, request, (short)(MAIN_SECTION_SIZE + 1 + module.length()));
        this.module = module;
    }

    /**
     * 'FIRST' message constructor
     * @param request unique request id
     * @param module module identifier. max 255 chars
     * @param parts number of parts
     * @param size full size of payload
     * @param accumulate accumulation flag
     */
    public BotControlMessageHeader(long request, String module, short parts, int size, boolean accumulate){
        this(
                accumulate ? MessageType.FIRST_WITH_ACCUMULATION_FLAG : MessageType.FIRST,
                request, (short)(MAIN_SECTION_SIZE + 7 + module.length()) // 7 = PARTS(2bytes) + SIZE(4bytes) + STR_SIZE(1byte)
        );
        this.module = module;
        this.parts = parts;
        this.payloadSize = size;
    }

    // 'NEXT' message constructor
    public BotControlMessageHeader(long request, short sequence){
        this(MessageType.NEXT, request, (short)(MAIN_SECTION_SIZE + 2)); // where 2 is a size of short
        this.sequence = sequence;
    }


    // 'LAST' message constructor
    public BotControlMessageHeader(long request){
        this(MessageType.LAST, request, MAIN_SECTION_SIZE); // last message doesn't have additional section
    }



    public byte[] serialize() throws RuntimeException {

        try{
            ByteBuffer buffer = ByteBuffer.allocate(headerSize);

            //    /- - - - - - - -MAIN SECTION - - - - - - - -\
            //    | 3 bytes | 2 bytes |  8 bytes   |  1 byte  | <- 14 bytes in total
            //    |  MAGIC  | H. SIZE | REQUEST ID | MSG TYPE |

            buffer.put("RCM".getBytes());  // bytes [1, 2, 3]
            buffer.putShort(headerSize);   // bytes [4, 5]
            buffer.putLong(request);       // bytes [6, 7, ...13]
            buffer.put(type.asByte);       // byte 14

            // serialize additional section
            switch (type) {
                case SINGLE -> {
                    // serialization of module identifier
                    buffer.put((byte) module.length());
                    buffer.put(module.getBytes());
                }
                case FIRST, FIRST_WITH_ACCUMULATION_FLAG -> {
                    // serialization number of parts + full payload size
                    buffer.putShort(parts);
                    buffer.putInt(payloadSize);

                    // serialization of module identifier
                    buffer.put((byte) module.length());
                    buffer.put(module.getBytes());
                }
                case NEXT -> buffer.putShort(sequence);
                case LAST -> {} // Noting to add
            }
            return buffer.array();
        } catch (RuntimeException e){
            throw new RuntimeException("Failed to serialize: " + e.getMessage());
        }
    }


    /**
     * Basic, private constructor
     * @param type message type
     * @param request request id
     * @param headerSize calculated header size (14 bytes main section + size of additional section)
     */
    private BotControlMessageHeader(MessageType type, long request, short headerSize){
        this.type = type;
        this.request = request;
        this.headerSize = headerSize;
    }

}
