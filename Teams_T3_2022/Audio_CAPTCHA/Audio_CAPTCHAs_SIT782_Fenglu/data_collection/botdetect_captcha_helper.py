from  data_collection import *

audio_id = 'demoCaptcha_SoundLink'
refresh_id = 'demoCaptcha_ReloadIcon'
picture_id = 'demoCaptcha_CaptchaImageDiv'

class LocateByIdStrategy(MenuLocateStrategy):
    def __init__(self,driver, id):
        self.driver = driver
        self.id = id
        self.action = ActionChains(driver)
    @sleep_decorator
    def locate_right_click(self):
        element = get_btn_by_id(self.driver, self.id)
        self.action.context_click(element).perform()


class BotdetectAudioDownload(Download_operation):
    def select_from_context_menu(self):
        for i in range(5):
            press_key_short('down')
        press_key_short('enter')

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
        cls.driver.get(cls.url)

    def download(self, fname):
        picture_downloader = BotdetectPictureDownloader().get_downloader()
        audio_downloader = BotdetectAudioDownloader().get_downloader()

        picture_downloader.download(fname)
        audio_downloader.download(fname)

    def refresh(cls):
        ele_refresh = get_btn_by_id(cls.driver, refresh_id)
        ele_refresh.click()
