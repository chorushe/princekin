from mitmproxy import http

def response(flow) :
    print("-----START-----",end = '')
    print(flow.request.url,end = '')
    print("-&&-",end = '')
    print(flow.request.http_version,end = '')
    print("-&&-",end = '')
    method=flow.request.method
    print(method,end = '')
    print("-&&-",end = '')
    print ((flow.response.timestamp_end-flow.request.timestamp_start)*1000,end = '')
    print("-&&-",end = '')
    h=flow.response.headers
    if 'Content-Type' in h:
        print(h['Content-Type'],end = '')
    else :
        print('XXX',end = '')
    print("-&&-",end = '')
    if method == "POST" :
        str1=flow.request.data.content
        print(str1.decode('utf-8'),end = '')
    print("-&&-",end = '')
    print(flow.response.status_code,end = '')
    print("-&&-",end = '')
    str=flow.response.content
    print(str.decode("utf-8"),end = '')
    print("-----END-----")
