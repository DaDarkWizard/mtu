from pathlib import Path
import asyncio
from playwright.async_api import async_playwright, TimeoutError as PlaywrightTimeoutError
from subprocess import Popen, DEVNULL

async def main():
    homedirectory = Path.home()

    config_file = open(str(homedirectory) + '/.mtu/config')

    config = {}

    for line in config_file.readlines():
        if len(line.strip()) <= 0:
            continue
        item = line.split(' ')[0]
        value = line[len(item) + 1:-1]
        config[item] = value

    session_id = ''

    async with async_playwright() as playwright:
        browser = await playwright.chromium.launch()
        # Load the cookies
        context = await browser.new_context()
        with open("cookies.json", "r") as f:
            cookies = json.loads(f.read())
            await context.add_cookies(cookies)
        page = await browser.new_page()
        await page.goto("https://vpn.mtu.edu")
        await page.get_by_label('Username').fill(config['mtu_id'])
        await page.get_by_label('Password').fill(config['mtu_password'])
        await page.locator('input', has_text='Logon').click()
        await page.locator('button', has_text='Other options').click()
        await page.locator('a', has_text='Duo Push').click()
        await page.locator('button', has_text='Yes, this is my device').click()
        try:
            await page.get_by_title('Access MTU Network 010').focus()
        except PlaywrightTimeoutError:
            await page.screenshot(path="test.png")
        session_id = await page.evaluate('() => document.cookie.match(/MRHSession=(.*?); /)[1]')
        # Save the cookies
        with open("cookies.json", "w") as f:
            await f.write(json.dumps(context.cookies()))
        await browser.close()

    return session_id

session_id = asyncio.run(main())

pid = Popen(['gof5', '--server', 'vpn.mtu.edu', '--session', session_id], stdout=DEVNULL, stdin=DEVNULL, stderr=DEVNULL).pid
print(pid)
