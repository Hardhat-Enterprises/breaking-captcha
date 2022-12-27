import random
import time

import pyautogui

from  data_collection import *

def random_position_from_range(x_range, y_range):
    x = random.uniform(*x_range)
    y = random.uniform(*y_range)
    return x, y


# the following coordinates represent element areas [(upper_left_x,upper_left_y),(lower_right_x, lower_right_y)] 
class State:
    btns_first_state = [[(89, 294), (414, 475)], [(200, 220), (662, 687)]]
    btns_normal_state = [[(152,170),(546,560)]]
    btn_audio = [[(265, 275), (494, 510)]]

    def __init__(self, strategy):
        self.strategy = strategy

    def btn_click(self, btns, button):
        # locate button
        for coordinate_ranges in btns:
            coordinate_x, coordiante_y = random_position_from_range(*coordinate_ranges)
            mouse_move_to(coordinate_x, coordiante_y)
        # click button
            mouse_click(button=button)

# each time, when start downloading, url is used
class FirstState(State):

    def prompt_download_context(self):
        self.strategy.state = self.strategy.normal_state
        self.btn_click(self.btns_first_state, 'left')
        self.btn_click(self.btn_audio,'right')

# each time, when a page has opened, url is not used, press refresh to continue downloading the next audio file
class NormalState(State):
    def prompt_download_context(self):
        self.btn_click(self.btns_normal_state,'left')
        self.btn_click(self.btn_audio, 'right')


# pyautogui is used instead of selenium's find_element method
class LocateByPositionStrategy(MenuLocateStrategy):
    def __init__(self):
        super().__init__()
        self.state = FirstState(self)
        self.normal_state = NormalState(self)

    def locate_right_click(self):
        self.state.prompt_download_context()


class GoogleRecaptchaDownload(Download_operation):

    def change_strategy(self, new_strategy):
        self.locate_strategy = new_strategy

    def select_from_context_menu(self):
        for i in range(4):
            press_key('down')
        press_key('enter')


class GoogleAudioDownloader(CaptchaFactory):
    downloader = GoogleRecaptchaDownload(LocateByPositionStrategy())

    @classmethod
    def get_downloader(cls):
        return cls.downloader


class GoogleTask(DataCollectionTask):

    url = DataCollectionTask.url_pool.get('google')

    def load_webpage(cls):
        cls.driver.get(cls.url)

    def download(self, fname):
        downloader = GoogleAudioDownloader().get_downloader()
        downloader.download(fname)

    def refresh(cls):
        time.sleep(random.uniform(3, 5))
        x = random.uniform(152,170)
        y = random.uniform(545,560)
        pyautogui.moveTo(x, y)
        mouse_click()






