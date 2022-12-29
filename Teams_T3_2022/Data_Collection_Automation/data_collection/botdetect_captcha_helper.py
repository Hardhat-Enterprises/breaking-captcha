from  data_collection import *
from selenium.webdriver.support.ui import Select

audio_id = 'demoCaptcha_SoundLink'
refresh_id = 'demoCaptcha_ReloadIcon'
picture_id = 'demoCaptcha_CaptchaImageDiv'

# used to select CAPTCHA length
minCode = 'minCodeLength'
maxCode = 'maxCodeLength'
applyBtn = 'applyButton'

#find element on webpage
class LocateByIdStrategy(MenuLocateStrategy):
    def __init__(self,driver, id):
        self.driver = driver
        self.id = id
        self.action = ActionChains(driver)
    @sleep_decorator
    def locate_right_click(self):
        element = get_btn_by_id(self.driver, self.id)
        self.action.context_click(element).perform()

#keyboard operations
class BotdetectAudioDownload(Download_operation):
    def select_from_context_menu(self):
        for i in range(5):
            press_key_short('down')
        press_key_short('enter')

#keyboard operations
class BotdetectPictureDownload(Download_operation):
    def select_from_context_menu(self):
        press_key_short('down')
        press_key_short('down')
        press_key_short('enter')


class BotdetectAudioDownloader(CaptchaFactory):
    downloader = BotdetectAudioDownload(LocateByIdStrategy(DataCollectionTask.driver,audio_id))

    @classmethod
    def get_downloader(cls):
        return cls.downloader

class BotdetectPictureDownloader(CaptchaFactory):
    downloader = BotdetectPictureDownload(LocateByIdStrategy(DataCollectionTask.driver,picture_id))

    @classmethod
    def get_downloader(cls):
        return cls.downloader


class BotdetectTask(DataCollectionTask):
    url = DataCollectionTask.url_pool.get('botdetect')

    def load_webpage(cls):
        # change setting before downloading
        cls.driver.get(cls.url)
        # now we are only interested in CAPTCHAs that are of length1
        select = Select(cls.driver.find_element(By.ID, minCode))
        # select by visible text
        select.select_by_visible_text('1')
        select = Select(cls.driver.find_element(By.ID, maxCode))
        # select by visible text
        select.select_by_visible_text('1')

        # click apply button
        ele_refresh = get_btn_by_id(cls.driver, applyBtn)
        ele_refresh.click()

    def download(self, fname):
        picture_downloader = BotdetectPictureDownloader().get_downloader()
        audio_downloader = BotdetectAudioDownloader().get_downloader()

        picture_downloader.download(fname)
        audio_downloader.download(fname)

    def refresh(cls):
        ele_refresh = get_btn_by_id(cls.driver, refresh_id)
        ele_refresh.click()
