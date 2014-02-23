BOT_NAME = 'scraper'

SPIDER_MODULES = ['scraper.spiders']
NEWSPIDER_MODULE = 'scraper.spiders'

ITEM_PIPELINES = {
	'scraper.files.FilesPipeline' : 0,
	# 'apkSpider.pipelines.SQLiteStorePipeline' : 0,
}

DOWNLOAD_DELAY = 0.25

FILES_STORE = './downloads'

COOKIES_ENABLED = False

# Crawl responsibly by identifying yourself (and your website) on the user-agent
# USER_AGENT = 'apkSpider (+http://www.yourdomain.com)'