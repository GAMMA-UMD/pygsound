import numpy as np
import pygsound as ps
from wavefile import WaveWriter, Format


def compute_array(meshpath, src_coord, lis_coord, r, s, micarray):
    mesh = ps.loadobj(meshpath, r, s)
    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.mono
    scene = ps.Scene()
    scene.setMesh(mesh)

    src = ps.Source(src_coord)
    src.radius = 0.01

    res = {}
    res_buffer = []
    rate = 0
    for offset in micarray:
        lis = ps.Listener((offset + lis_coord).tolist())
        lis.radius = 0.01

        res_ch = scene.computeIR([src], [lis], ctx)
        rate = res_ch['rate']
        sa = res_ch['samples'][0][0][0]
        res['rate'] = rate
        res_buffer.append(sa)
    res['samples'] = np.zeros((len(res_buffer), np.max([len(ps) for ps in res_buffer])))
    for i, c in enumerate(res_buffer):
        res['samples'][i, :len(c)] = c
    return res


def main():
    custom_array = np.array(
        [[0, 0.035, 0], [-0.0303, 0.0175, 0], [-0.0303, -0.0175, 0], [0, -0.035, 0], [0.0303, -0.0175, 0],
         [0.0303, 0.0175, 0]])

    src_coord = [1, 1, 1]
    lis_coord = [0.1, 0.1, 0.1]

    res = compute_array("cube.obj", src_coord, lis_coord, 0.5, 0.1, custom_array)

    with WaveWriter('custom_array.wav', channels=np.shape(res['samples'])[0], samplerate=int(res['rate'])) as w:
        w.write(np.array(res['samples']))


if __name__ == '__main__':
    main()
