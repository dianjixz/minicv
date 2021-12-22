import os
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

print(get_srcs('src/omv/img', ["c"], ['mjpeg.c','orb.c','bayer.c','font.c','gif.c','lbp.c','fft.c','jpeg.c','bmp.c','jpegd.c','ppm.c','conv.c','picojpeg_util.c','tempCodeRunnerFile.c']))
