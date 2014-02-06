#!/usr/bin/env python
import spynner
import pyquery

browser = spynner.Browser(debug_level=spynner.DEBUG)
browser.create_webview()
#browser.show()
browser.set_html_parser(pyquery.PyQuery)
browser.load("http://detail.tmall.com/item.htm?spm=a230r.1.10.2.cpNxpf&id=19955188752&ad_id=&am_id=&cm_id=&pm_id=")

browser.wait(2)
print "url:", browser.url

# Soup is a PyQuery object
browser.soup.make_links_absolute(base_url=browser.url)
print "html:", browser.soup("#J_PromoBox").html()#这个id的数据就是js加载出来的

# Demonstrate how to download a resource using PyQuery soup
#imagedata = browser.download(browser.soup("img:first").attr('src'))
#print "image length:", len(imagedata))
browser.close()
