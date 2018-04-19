from mitmproxy import http

def response(flow) :

    res='-----START-----'
    #print("-----START-----",end = '')
    #print(flow.request.url,end = '')
    res+=flow.request.url
    res+="-&&-"
    res+=flow.request.http_version
    res+="-&&-"
    
    method=flow.request.method
    res+=method
    res+="-&&-"
    res+=str((flow.response.timestamp_end-flow.request.timestamp_start)*1000)
    res+="-&&-"
    encoding = 'utf-8'
    h=flow.response.headers
    if 'Content-Type' in h:
        contentType =h['Content-Type']
        res+=contentType
        index=contentType.find('charset=')
        if index!=-1:
            encoding=contentType[index+8:]
            #print(encoding, file=open("encodings.txt", "a"))

    else :
        res+='XXX'
    res+="-&&-"
    if method == "POST" :
        str1=flow.request.data.content
        try:
            res+=str1.decode(encoding)
        except UnicodeDecodeError:
            res+='小王子无法解析'
    res+="-&&-"
    res+=str(flow.response.status_code)
    res+="-&&-"
    str2=flow.response.content
    try:
        res+=str2.decode(encoding)
    except UnicodeDecodeError:
        res+='小王子无法解析'
        
    res+="-----END-----"
    print(res)
   