#!/bin/env python2.7
# -*- coding: utf-8 -*-

import json

var = '{"content":" 服装批发 低至50元2件","img":"http://d2.sina.com.cn/pfpghc2/201609/15/41a81f7945774aedbce10b699442a7fa.jpg","json":"{\\"clickURL\\":\\"https://mingyin.1688.com\\",\\"IdeaContentInput\\":\\" 服装批发 低至50元2件\\"}"}' 

obj = json.loads(var)

print json.loads(obj['json'])
