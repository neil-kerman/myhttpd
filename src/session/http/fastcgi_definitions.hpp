

/*
 * Number of bytes in a FCGI_Header.  Future versions of the protocol
 * will not reduce this number.
 */
#define FCGI_HEADER_LEN  8

 /*
  * Value for version component of FCGI_Header
  */
#define FCGI_VERSION_1           1

  /*
   * Values for type component of FCGI_Header
   */
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)

   /*
    * Value for requestId component of FCGI_Header
    */
#define FCGI_NULL_REQUEST_ID     0



/*
 * Mask for flags component of FCGI_BeginRequestBody
 */
#define FCGI_KEEP_CONN  1

 /*
  * Values for role component of FCGI_BeginRequestBody
  */
#define FCGI_RESPONDER  1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER     3



/*
 * Values for protocolStatus component of FCGI_EndRequestBody
 */
#define FCGI_REQUEST_COMPLETE 0
#define FCGI_CANT_MPX_CONN    1
#define FCGI_OVERLOADED       2
#define FCGI_UNKNOWN_ROLE     3

 /*
  * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
  */
#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"



namespace myhttpd::http {

    struct fixed_header {

        unsigned char version;

        unsigned char type;

        unsigned char requestIdB1;

        unsigned char requestIdB0;

        unsigned char contentLengthB1;

        unsigned char contentLengthB0;

        unsigned char paddingLength;

        unsigned char reserved;

        inline unsigned short get_requestid() {
            return (this->requestIdB1 << 8) || this->requestIdB0;
        }

        inline void set_requestid(unsigned short id) {
            this->contentLengthB0 = id;
            this->contentLengthB1 = id >> 8;
        }

        inline unsigned short get_content_langth() {
            return (this->contentLengthB1 << 8) || this->requestIdB0;
        }

        inline void set_content_length(unsigned short len) {
            this->contentLengthB0 = len;
            this->contentLengthB1 = len >> 8;
            this->paddingLength = 8 - this->get_content_langth() % 8;
        }
    };

    struct begin_request_body {

        unsigned char roleB1;

        unsigned char roleB0;

        unsigned char flags;

        unsigned char reserved[5];
    } ;

    struct begin_request_record {

        fixed_header header;

        begin_request_body body;
    };

    struct end_request_body {

        unsigned char appStatusB3;

        unsigned char appStatusB2;

        unsigned char appStatusB1;

        unsigned char appStatusB0;

        unsigned char protocolStatus;

        unsigned char reserved[3];
    };

    struct end_request_record {

        fixed_header header;

        end_request_body body;
    };

    struct unknown_type_body {

        unsigned char type;

        unsigned char reserved[7];
    };

    struct unknown_type_record {

        fixed_header header;

        unknown_type_body body;
    };
    
}
