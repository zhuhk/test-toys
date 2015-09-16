#!/bin/env python
import pycurl

# -*- coding: utf-8 -*-  
import pycurl  
import os  
import StringIO

def upload():  
  pc = pycurl.Curl()  
  pc.setopt(pycurl.POST, 1)  
  pc.setopt(pycurl.URL, 'http://127.0.0.1/')  
  data = "test"
  pc.setopt(pycurl.POSTFIELDS, data)  
  pc.setopt(pycurl.VERBOSE, 1);

  b = StringIO.StringIO()
  pc.setopt(pycurl.WRITEFUNCTION, b.write)
  for _ in range(10000):
    pc.perform()  
  #response_code = pc.getinfo(pycurl.RESPONSE_CODE)  
  pc.close()  

if __name__ == "__main__":  
  upload()
