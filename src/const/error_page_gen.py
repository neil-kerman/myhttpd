import sys

error_codes = (
    {"code":400, "text":"Bad Request"},
    {"code":401, "text":"Unauthorized"},
    {"code":402, "text":"Payment Required"},
    {"code":403, "text":"Forbidden"},
    {"code":404, "text":"Not Found"},
    {"code":405, "text":"Method Not Allowed"},
    {"code":406, "text":"Not Acceptable"},
    {"code":407, "text":"Proxy Authentication Eequired"},
    {"code":408, "text":"Request Timeout"},
    {"code":409, "text":"Conflict"},
    {"code":410, "text":"Gone"},
    {"code":411, "text":"Length Required"},
    {"code":412, "text":"Precondition Failed"},
    {"code":413, "text":"Request Entity Too Large"},
    {"code":414, "text":"Request-uri Too Long"},
    {"code":415, "text":"Unsupported Media Type"},
    {"code":416, "text":"Requested Range Not Satisfiable"},
    {"code":417, "text":"Expectation Failed"},
    {"code":500, "text":"Internal Server Error"},
    {"code":501, "text":"Not Implemented"},
    {"code":502, "text":"Bad Gateway"},
    {"code":503, "text":"Service Unavailable"},
    {"code":504, "text":"Gateway Timeout"},
    {"code":505, "text":"Http Version Not Supported"}
)

tem_page_file = open("error_page_template.html", "r")
tem_page = tem_page_file.read()
tem_page_file.close()

for error_code in error_codes:
    content = tem_page.format(error_code["code"], error_code["text"])
    open("")

tem_page.format()