import os
import sys
from playwright.sync_api import sync_playwright

def capture_html(html_path, output_path, width=1280, height=650):
    html_uri = f"file:///{html_path.replace(os.sep, '/')}"
    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True)
        page = browser.new_page()
        page.set_viewport_size({"width": width, "height": height})
        page.goto(html_uri)
        # Wait a bit for fonts locally if needed, though system fonts usually render immediately
        page.wait_for_timeout(500)
        page.screenshot(path=output_path, full_page=True)
        browser.close()
    
    print(f"Screenshot saved to {output_path}")

if __name__ == '__main__':
    html_file = r"c:\GIThub coding\1. F-07,F-08 개발\MBP-T05-ChildLock\tmp\slide5_fixed.html"
    out_file = r"c:\GIThub coding\1. F-07,F-08 개발\MBP-T05-ChildLock\slide5_fixed.png"
    capture_html(html_file, out_file)
