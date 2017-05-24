import httplib

httplib.HTTPConnection.debuglevel = 1

class CurlTool(object):
  def __init__(self):
    self.cache = {}

  def LoadCache(self, fname):
    f = open(fname)
    for line in f:
      k,v = line.strip().split('\t')
      self.cache[k] = v
    f.close()

  def GetHostPath(self, clickurl):
    pos = clickurl.find('://')
    if pos > 0:
      clickurl = clickurl[pos+3:]
    host = clickurl
    path = '/'
    pos = clickurl.find('/')
    if pos > 0:
      host = clickurl[:pos]
      path = clickurl[pos:]
    else:
      pos = clickurl.find('?')
      if pos > 0:
        host = clickurl[:pos]
    return host, path

  def GetRealURL(self, clickurl):
    host, path = self.GetHostPath(clickurl)
    if self.cache.get(host, 1) == 0:
      return clickurl
    print host, path
    conn = httplib.HTTPConnection(host)
    conn.request('GET', path)
    resp = conn.getresponse()
    print resp.status
    if resp.status >= 300 and resp.status < 400:
      for item in resp.getheaders():
        if item[0] == 'location':
          clickurl = item[1]
          break
    conn.close()
    return clickurl

if __name__ == '__main__':
  ct = CurlTool()
  #print ct.GetRealURL('http://www.liangzixuan.com?gzid=pc4xw')
  print ct.GetRealURL('http://api.game487.cn/api/adv/tuiguang.php?channel=momo17')
