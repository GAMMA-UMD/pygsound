import numpy as np
import pygsound as ps
import matplotlib.pyplot as plt
import rir_generator as rir
# using https://github.com/audiolabs/rir-generator/


def main():
    l = 10
    w = 6
    h = 4
    absorb = 0.3
    reflec = np.sqrt(1.0 - absorb)

    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.mono
    scene = ps.Scene()

    mesh = ps.createbox(l, w, h, absorb, 0.1)
    scene.setMesh(mesh)

    src_coord = [1, 1, 0.5]
    lis_coord = [5, 3, 0.5]

    src = ps.Source(src_coord)
    src.radius = 0.01

    lis = ps.Listener(lis_coord)
    lis.radius = 0.01

    rir_gs = scene.computeIR([src], [lis], ctx)
    rir_img = rir.generate(
        c=343,  # Sound velocity (m/s)
        fs=ctx.sample_rate,  # Sample frequency (samples/s)
        r=[lis_coord],
        s=src_coord,  # Source position [x y z] (m)
        L=[l, w, h],  # Room dimensions [x y z] (m)
        beta=[reflec] * 6,  # Reflection coefficients
    )
    rir_img = rir_img[:, 0] / max(abs(rir_img))
    rir_gs = rir_gs['samples'][0][0][0]
    max_cnt = min(len(rir_gs), len(rir_img))

    fig, axs = plt.subplots(2, 1, figsize=(16, 10))
    axs[0].set_title('Image Method (waveform)')
    axs[0].plot(rir_img, linewidth=0.5)
    axs[0].set_xlabel('Sample')
    axs[0].set_xlim(0, max_cnt)
    axs[0].set_ylim(-1, 1)
    axs[0].set_ylabel('Amplitude')

    axs[1].set_title('Geometric Sound Propagation (waveform)')
    axs[1].plot(rir_gs, linewidth=0.5)
    axs[1].set_xlabel('Sample')
    axs[1].set_xlim(0, max_cnt)
    axs[1].set_ylim(-1, 1)
    axs[1].set_ylabel('Amplitude')

    fig.tight_layout()
    plt.savefig('img_comparison.png')
    plt.show()


if __name__ == '__main__':
    main()
