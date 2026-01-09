from pathlib import Path
import asyncio
import json
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
        # Create context with attached storage if exists
        storage_path = Path("storage.json")
        if storage_path.exists():
            try: 
                context = await browser.new_context(storage_state=storage_path)
            except Exception as e: 
                context = await browser.new_context()
                print("Failed to load storage ERROR {e}")
        else:
            context = await browser.new_context()
        # Open browser and go to vpn.mtu.edu and sign in
        page = await context.new_page()
        await page.goto("https://vpn.mtu.edu")
        # Try sign in if no storage
        if await page.get_by_label('Username').is_visible():
            try:
                await page.get_by_label('Username').fill(config['mtu_id'])
                await page.get_by_label('Password').fill(config['mtu_password'])
                await page.locator('input', has_text='Logon').click()
                print("Please accept DUO prompt")
                # Handle Duo 2FA Options
                #await page.locator('button', has_text='Other options').click()
                #await page.locator('a', has_text='Duo Push').click()
                # If trust button is visible click it
                button = page.locator("#trust-browser-button")
                try:
                    await button.wait_for(state="visible", timeout=20000)
                    await button.click()
                except Exception:
                    print("Duo prompt did not appear")
            except Exception as e:
                print(f"ERROR {e}")
                
        # Save the storage 
        print("Saving storage")
        await context.storage_state(path="storage.json")
        print("Done!")
        # Activate vpn 
        try:
            await page.get_by_title('Access MTU Network 010').focus()
        except PlaywrightTimeoutError:
            await page.screenshot(path="test.png")
        session_id = await page.evaluate('() => document.cookie.match(/MRHSession=(.*?); /)[1]')
        await browser.close()

    return session_id

session_id = asyncio.run(main())

pid = Popen(['gof5', '--server', 'vpn.mtu.edu', '--session', session_id], stdout=DEVNULL, stdin=DEVNULL, stderr=DEVNULL).pid
print(pid)
