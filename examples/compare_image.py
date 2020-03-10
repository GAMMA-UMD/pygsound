import numpy as np
import pygsound as ps
import matplotlib.pyplot as plt
import rirgenerator as rg


def main():
    l = 10
    w = 6
    h = 2
    absorb = 0.3
    reflec = np.sqrt(1.0 - absorb)

    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.mono
    scene = ps.Scene()

    mesh = ps.createbox(l, w, h, absorb, 0.5)
    scene.setMesh(mesh)

    src_coord = [1, 1, 0.5]
    lis_coord = [5, 3, 0.5]

    src = ps.Source(src_coord)
    src.radius = 0.01

    lis = ps.Listener(lis_coord)
    lis.radius = 0.01

    rir_gs = scene.computeIR(src, lis, ctx)
    rir_img = rg.rir_generator(343, 16000, lis_coord, src_coord, [l, w, h], beta=[reflec]*6)
    rir_img = rir_img / max(abs(rir_img[0]))

    max_cnt = min(len(rir_gs['samples']), len(rir_img[0])) * 2

    fig, axs = plt.subplots(4, 1, figsize=(10, 20))
    axs[0].set_title('Image Method (waveform)')
    axs[0].plot(rir_img[0], linewidth=0.5)
    axs[0].set_xlabel('Sample')
    axs[0].set_xlim(0, max_cnt)
    axs[0].set_ylim(-1, 1)
    axs[0].set_ylabel('Amplitude')

    axs[1].set_title('Geometric Sound Propagation (waveform)')
    axs[1].plot(rir_gs['samples'], linewidth=0.5)
    axs[1].set_xlabel('Sample')
    axs[1].set_xlim(0, max_cnt)
    axs[1].set_ylim(-1, 1)
    axs[1].set_ylabel('Amplitude')

    axs[2].set_title('Image Method (spectrogram)')
    axs[2].specgram(rir_img[0], mode='magnitude', NFFT=1024, Fs=16000, noverlap=512)
    axs[2].set_xlim(0, max_cnt / 16000)
    axs[2].set_xlabel('Times (s)')
    axs[2].set_ylabel('Frequency (Hz)')

    axs[3].set_title('Geometric Sound Propagation (spectrogram)')
    axs[3].specgram(rir_gs['samples'], mode='magnitude', NFFT=1024, Fs=16000, noverlap=512)
    axs[3].set_xlim(0, max_cnt / 16000)
    axs[3].set_xlabel('Times (s)')
    axs[3].set_ylabel('Frequency (Hz)')

    fig.tight_layout()
    plt.savefig('img_comparison.png')
    plt.show()


if __name__ == '__main__':
    main()
