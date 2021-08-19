from setuptools import setup, find_packages, Extension

import os


def get_incs(path, exclude=[]):
    setup_path = os.path.dirname(os.path.abspath(__file__)) + '/' # ./
    return setup_path + path

def get_srcs(path, file_type=['c', 'cpp', 'cc', 'cxx', 's'], exclude=[]):
    _sources = []
    setup_path = os.path.dirname(os.path.abspath(__file__)) + '/' # ./
    # print(setup_path + path)
    for root, dirs, files in os.walk(setup_path + path):
        for file in files:
            _type = file.split(".")
            if len(_type) > 1 and _type[1].lower() in file_type:
                abs_path = root.replace(setup_path, '')
                _sources.append(abs_path + '/' + file)
            elif file in file_type:
                abs_path = root.replace(setup_path, '')
                _sources.append(abs_path + '/' + file)
    if len(exclude):
        # _sources = list(set(_sources).difference(set(exclude)))
        # print('_sources', len(_sources))
        for s in range(len(_sources)-1, -1, -1):
          for e in exclude:
            if e in _sources[s]:
                _sources.pop(s)
                break
            # else:
            #     print(_sources[s])
        # print('res', _sources)
    return _sources

ext_modules = Extension(
    name = 'minicv',
    include_dirs = [
        get_incs('src/omv/img/include'),
        get_incs('src/omv/include'),
        get_incs('src/py_modules')
        # get_incs('micropython/py'),
        # get_incs('openmv/src/omv/imlib'),
        # get_incs('openmv/src/omv/common'),
        # get_incs('openmv/src/omv/alloc'),
        # get_incs('openmv/src/omv/modules'),
        # get_incs('openmv/src/hal/cmsis/include'),
        # get_incs('ports/unix'),
        # get_incs('ports/unix/variants/minimal'),
        # get_incs('ports/unix/build-standard/'),
    ],
    sources = (
        get_srcs('src', ["c"], [
            'mjpeg.c',
            'orb.c',
            'bayer.c',
			'font.c',
			'gif.c',
			'lbp.c',
			'fft.c',
			'jpeg.c',
			'bmp.c',
			'jpegd.c',
			'ppm.c',
			'conv.c',
			'picojpeg_util.c',
			'vfs_wrapper.c',
			'py_clock.c',
			'py_fir.c',
			'py_gif.c',
			'py_helper.c',
			'mp_py_image.c',
			'py_imageio.c',
			'py_tf.c',
			'image0.c',
			'py_lcd.c',
			'py_mjpeg.c',
			'py_omv.c',
			'py_sensor.c',
			'haar.c',
			'tempCodeRunnerFile.c'
        ]) 
        # get_srcs('src/omv', ["c"], [
        #     'vfs_wrapper.c',
		# 	'tempCodeRunnerFile.c'
        # ]) +
		# get_srcs('src/py_modules', ["c"])

    ),
    libraries=[
        
    ],
    # library_dirs=[ "./ports"],
    extra_compile_args=['-lm'], # V83x use armv7 -DARMV7_V83X , '-DARMV7_V83X', '-DARM_MATH_DSP'
    # extra_link_args  = [ "-Wl,-rpath=/usr/lib/python3.8/site-packages/xxxxxx" ]
)


setup(
    name="minicv",
    version="0.1.1",
    description="A Library For minicv Example",
    author="dianjixz",
    author_email="18637716021@sipeed.com",
    license="BSD",
    url="https://github.com/dianjixz/minicv.git",
    packages=find_packages(),
    install_requires=["Pillow", "numpy"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: BSD License",
    ],
    ext_modules = [ 
        # omv_module, 
        ext_modules, 
    ],
    setup_requires=[
        'pytest-runner',
    ],
    tests_require=[
        'pytest',
    ],
)










# get_srcs('src/omv/img', ["c"], ['mjpeg.c','orb.c','bayer.c','font.c','gif.c','lbp.c','fft.c','jpeg.c','bmp.c','jpegd.c','ppm.c','conv.c','picojpeg_util.c','tempCodeRunnerFile.c'])