# -*- coding: utf-8 -*-
import spynner

if __name__ == "__main__":
    browser = spynner.Browser()
    # 设置代理
    #browser.set_proxy('http://host:port')    browser.show()
    try:
	browser.load(url='http://duckduckgo.com', load_timeout=120, tries=1)
    except spynner.SpynnerTimeout:
	print 'Timeout.'
    else:
	# 输入搜索关键字
	browser.wk_fill('input[id="search_form_input_homepage"]', 'something')
	# 点击搜索按钮，并等待页面加载完毕
	browser.wk_click('input[id="search_button_homepage"]', wait_load=True)
	# 获取页面的HTML
	html = browser.html
	if html:
	    html = html.encode('utf-8')
	    open('t_results1.html', 'w').write(html)
    browser.close()

