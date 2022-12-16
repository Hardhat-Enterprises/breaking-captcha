from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from selenium.webdriver import ActionChains
from selenium.webdriver.common.keys import Keys
import pyautogui
import time
from abc import ABC, abstractmethod
import random

class CaptchaFactory(ABC):
    @abstractmethod
    def get_downloader(self):
        pass

def get_btn_by_id(driver, id):
    return WebDriverWait(driver, timeout=20).until(
        EC.element_to_be_clickable((By.ID, id)))

def sleep_decorator(original_function):
    def wrapper_function(*args, **kwargs):
        # always sleep a random second before action
        time.sleep(random.uniform(2, 4))
        result = original_function(*args, **kwargs)
        time.sleep(random.uniform(2, 4))
        return result
    return wrapper_function


def sleep_decorator_short(original_function):
    def wrapper_function(*args, **kwargs):
        # always sleep a random second before action
        time.sleep(random.uniform(0.5,1))
        result = original_function(*args, **kwargs)

        return result

    return wrapper_function

press_key = sleep_decorator(pyautogui.press)
mouse_move_to = sleep_decorator(pyautogui.moveTo)
mouse_click = sleep_decorator(pyautogui.click)
write_filename = sleep_decorator(pyautogui.write)

press_key_short = sleep_decorator_short(pyautogui.press)
mouse_move_to_short = sleep_decorator_short(pyautogui.moveTo)
mouse_click_short = sleep_decorator_short(pyautogui.click)
write_filename_short = sleep_decorator_short(pyautogui.write)

class MenuLocateStrategy:
    @abstractmethod
    def locate_right_click(self):
        pass


class Download_operation(ABC):

    def __init__(self, locate_strategy):
        self.locate_strategy = locate_strategy

    def locate_right_click(self):
        self.locate_strategy.locate_right_click()

    @abstractmethod
    def select_from_context_menu(self):
        pass

    def save_file(self, fname):
        write_filename(str(fname), interval=0.25)
        press_key('enter')


    def download(self,fname):
        self.locate_right_click()
        self.select_from_context_menu()
        self.save_file(fname)

class DataCollectionTask(ABC):
    driver = webdriver.Chrome(service=Service('chromedriver.exe'))
    url_pool = {
        'botdetect': 'https://captcha.com/demos/features/captcha-demo.aspx',
        'google': 'https://recaptcha-demo.appspot.com/recaptcha-v2-checkbox-explicit.php'
    }

    @classmethod
    @abstractmethod
    def load_webpage(cls):
        pass

    @sleep_decorator
    @abstractmethod
    def download(self,fname):
        pass

    @classmethod
    @abstractmethod
    def refresh(cls):
        pass

    def do(self, fname_from, fname_to):
        for i in range(fname_from, fname_to+1):
            self.download(str(i))
            self.refresh()
            time.sleep(random.uniform(5,6))




