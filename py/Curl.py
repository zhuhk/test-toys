 
#!/bin/env python

# -*- coding: <encoding name> -*-

import pycurl,StringIO,time
import logging
import traceback

class Curl:
  c = pycurl.Curl()
  #c.setopt(c.VERBOSE, True)

  def __del__(self):
    self.c.close()

  def HttpGet(self, url):
    buffer = StringIO.StringIO()
    self.c.setopt(self.c.WRITEFUNCTION, buffer.write)
    self.c.setopt(self.c.URL, url)
    self.c.perform()
    return self.c, buffer

  def HttpGetRetry(self, url, retry=3, wait=10):
    retryCnt = 0
    while retryCnt < retry:
      try:
        c, buffer = self.HttpGet(url)
        code = c.getinfo(c.RESPONSE_CODE)
        if code == 200: 
          #logging.info("HttpGet succ. code:" + str(code) + " url:" + url)
          return True, c, buffer
        logging.info("HttpGet fail. code:" + str(code) + " url:" +url)
      except:
        logging.info("HttpGet fail. except url:" + url)
        traceback.print_exc()

      time.sleep(wait)
      retryCnt += 1
    return False, c, buffer

if __name__ == "__main__":
  logging.basicConfig(level=logging.INFO,format="%(asctime)s %(levelname)s %(filename)s:%(lineno)d,%(funcName)s - %(message)s")
  curl = Curl()
  c, buffer = curl.HttpGetRetry("http://www.baidu.com/")
  print('Status: %d' % c.getinfo(c.RESPONSE_CODE))
  print('URL: %s' % c.getinfo(c.EFFECTIVE_URL))
  print('ContLen: %s' % len(buffer.getvalue()))
  c, buffer = curl.HttpGetRetry("www.example/", 2, 1)
