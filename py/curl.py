#!/bin/env python

import pycurl
from StringIO import StringIO

buffer = StringIO()

c = pycurl.Curl()
c.setopt(c.VERBOSE, True)
c.setopt(c.WRITEFUNCTION, buffer.write)
c.setopt(c.URL, 'https://at.umeng.com/u0v85n')
c.setopt(c.FOLLOWLOCATION, True)
c.setopt(c.NOBODY, True)
c.perform()
print('Status: %d' % c.getinfo(c.RESPONSE_CODE))
print('Status: %s' % c.getinfo(c.EFFECTIVE_URL))
c.close()

