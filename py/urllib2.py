import random
import json
import urllib2
import threading
import time

import timeit
import pycurl
import StringIO

#f1 = open('/data0/cookie_data','r')
#f1 = open('/data0/perfdebug/Service/cookieId.txt','r')
f1 = open('cookieId.txt','r')
cookie_str = f1.readlines()

f1.close()


def run():
    print "do run"
    crl = pycurl.Curl()
    url = "http://10.13.88.164:8080/mfp/delivery.do"
    crl.setopt(pycurl.URL, url)
    while True:
        rotate_count = random.randrange(0, 1000000001, 1)
        cookie_id = random.choice(cookie_str)
        #print cookie_id
        postData = {
            "adunitId":"PDPS000000055622",
            "rotateCount":rotate_count,
            "hashCode":"35llllle29213f9605",
            "cookieId":cookie_id,
            "userId":"sina.com.cn%2F"+ str(rotate_count),
            "ip":"60.194.130.10",
            "pageUrl":"sax.sina.cn",
            "pageKeyword":"",
            "ua":"Mozilla/5.0 (Linux; U; Android 2.3.6; zh-cn; GT-I8150 Build/GINGERBREAD) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
            "version":"vt4"
        }     
    
        #url = "http://10.13.88.164:8080/mfp/delivery.do"
        json_postData = json.write(postData)
        #print "<<<<<<<<<<<<<post data>>>>>>>>>>>>>>>>>>>"
        #print json_postData
        crl.fp = StringIO.StringIO()
        crl.setopt(crl.WRITEFUNCTION, crl.fp.write)
        crl.setopt(pycurl.POST, 1)
        crl.setopt(pycurl.POSTFIELDS, json_postData)
        crl.perform()
        print int(time.time()), crl.getinfo(pycurl.HTTP_CODE), crl.getinfo(pycurl.TOTAL_TIME), crl.getinfo(pycurl.SIZE_DOWNLOAD), crl.getinfo(pycurl.REQUEST_SIZE)
        #print crl.fp.getvalue()
        #start_timer = timeit.default_timer()
        #request = urllib2.Request(url,json_postData)
        #resp = urllib2.urlopen(request,timeout=3) 
        #content = resp.read()
        #print "<<<<<<<content>>>>>>>>>>"   
        #print content
        #assert (resp.code == 200), 'Bad HTTP Response'
        #assert ('src' in content), 'Failed Content Verification:'+content
        #latency = timeit.default_timer() - start_timer
        #self.custom_timers['Example_Timer'] = latency
      
   


if __name__ == '__main__':
    #trans = Transaction()
    #trans.run()
    #print trans.custom_timers
    run()
    #trans.start()
    #print trans.custom_timers
