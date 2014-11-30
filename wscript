import os

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')
    ctx.load('autoconfig', tooldir='pebble-autoconfig/wtools')

def configure(ctx):
    ctx.load('pebble_sdk')
    ctx.load('autoconfig', tooldir='pebble-autoconfig/wtools')
    gbitmap_tools_path = ctx.path.find_dir('lib').abspath()
    ctx.check(stlib='gbitmap-tools', stlibpath=os.path.join(gbitmap_tools_path, "lib"), includes=os.path.join(gbitmap_tools_path, "include"), uselib_store='GBITMAP_TOOLS')

def build(ctx):
    ctx.load('pebble_sdk')
    ctx.load('autoconfig', tooldir='pebble-autoconfig/wtools')

    ctx.pbl_autoconfprogram(
        source=ctx.path.ant_glob(['src/**/*.c']),
        target='pebble-app.elf',
        use=['GBITMAP_TOOLS'])
        
    ctx.pbl_bundle(elf='pebble-app.elf', js='pebble-js-app.js')
