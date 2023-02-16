import pandas as pd
import matplotlib.pyplot as plt

from constants import EXP_DATA_DIR

# Only one.
KERNELS = ['histogram']
Q_SIZE = 8

plt.rcParams['font.size'] = 12
# colors = seaborn.color_palette("rocket", 3)
colors = ['#c3121e', '#0348a1', '#ffb01c', '#027608',
          '#0193b0', '#9c5300', '#949c01', '#7104b5']
# plt.figure(unique_id) ensures that different invocations of make_plot() don't interfere
fig_id = 0


def make_plot(df, memtype, kernel):
    global fig_id

    fig_id = fig_id + 1
    plt.style.use(f'{EXP_DATA_DIR}/.plot_style.txt')


    x_design = df['design'].values.tolist()
    freqs = df['freq'].values.tolist()

    fig = plt.figure(fig_id) # Create matplotlib figure

    ax = fig.add_subplot() # Create matplotlib axes
    ax2 = ax.twinx() # Create another axes that shares the same x-axis as ax.

    width = 0.2
    df.time.plot(kind='bar', backend='matplotlib', color='#b3de69', ax=ax, width=width, position=1)
    df.cycles.plot(kind='bar', backend='matplotlib', color='#fb8072', ax=ax2, width=width, position=0)

    ax.spines['right'].set_color('None') 
    ax.spines['right'].set_visible(False) 
    ax2.spines['right'].set_visible(True) 
    ax2.spines['left'].set_color('#b3de69') 
    ax2.spines['right'].set_color('#fb8072') 

    ax.set_ylabel('Time - ms')
    ax2.set_ylabel('Cycles')
    ax.legend(['Time in Hardware'], loc="upper left")
    ax2.legend(['Simulation Cycles'], loc="upper right")
    
    x_design = [f'{x_design[i]}\n{freqs[i]}MHz' for i in range(len(x_design))]
    ax.set_xticklabels(x_design, rotation=0)
    plt.tight_layout()

    plt.show()

    plt.savefig(f'{EXP_DATA_DIR}/ii_sim_hw_{memtype}_exp_{kernel}.pdf')


if __name__ == '__main__':
    for kernel in KERNELS:
        res_file_bram = f'{EXP_DATA_DIR}/ii_sim_hw_{kernel}_bram.csv'
        df_bram = pd.read_csv(res_file_bram)
        make_plot(df_bram, 'bram', kernel)

        res_file_dram = f'{EXP_DATA_DIR}/ii_sim_hw_{kernel}_dram.csv'
        df_dram = pd.read_csv(res_file_dram)
        make_plot(df_dram, 'dram', kernel)
