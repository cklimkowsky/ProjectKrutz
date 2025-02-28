from scrapy import log
from scrapy.contrib.spiders import CrawlSpider
from scrapy.selector import Selector
from scrapy.http import Request
from play import parse_app

class FDroidSpider(CrawlSpider):
    name = "fdroid"
    start_urls = ["https://f-droid.org/repository/browse/"]

    # Parses the F-Droid "Browse" page and subsequent pages
    def parse(self, response):
        sel = Selector(response)
        apk_page_urls = sel.xpath('//a[descendant::div[@id="appheader"]]/@href').extract()

        for url in apk_page_urls:
            yield Request(url, callback=self.parse_page)

        next_page = sel.xpath('//a[contains(@href, "page") and text()[contains(., "next")]]/@href').extract()
        
        try:
            yield Request(next_page[0])
        except IndexError:
            log.msg('Crawling last page, spider will close soon', level=log.INFO)
            return

    # Parses pages for individual APK files
    def parse_page(self, response):
        sel = Selector(response)
        download_url = sel.xpath('//a[".apk" = substring(@href, string-length(@href) - 3) and text()[contains(., "download apk")]]/@href').extract()
        app_id = response.url[response.url.find('id=') + 3:]
        google_play_url = 'https://play.google.com/store/apps/details?id=' + app_id
        
        if download_url and google_play_url:
            yield Request(google_play_url, meta={'url': response.url, 'file_urls': [download_url[0]], 'come_from': self.name}, callback=parse_app)
            