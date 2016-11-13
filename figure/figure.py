import matplotlib.pyplot as plt
from matplotlib.mlab import csv2rec
from matplotlib.cbook import get_sample_data

fname = 'results.csv'

data = csv2rec(fname)

color_sequence = ['#1f77b4', '#aec7e8', '#ff7f0e', '#ffbb78', '#2ca02c',
                  '#98df8a', '#d62728']

fig, ax = plt.subplots(1, 1, figsize=(12, 9))

ax.spines['top'].set_visible(False)
ax.spines['bottom'].set_visible(True)
ax.spines['right'].set_visible(False)
ax.spines['left'].set_visible(True)

ax.get_xaxis().tick_bottom()
ax.get_yaxis().tick_left()

plt.xlim(900, 10100)
plt.ylim(0.1, 0.45)

plt.xticks(range(1000, 11000, 1000), fontsize=14)
plt.yticks([x / 100.0 for x in range(10, 45, 5)], ['{0}'.format(x) for x in range(10, 45, 5)], fontsize=14)

for y in [x / 100.0 for x in range(10, 45, 5)]:
    plt.plot(range(900, 10100), [y] * len(range(900, 10100)), '--',
             lw=0.5, color='black', alpha=0.3)

plt.tick_params(axis='both', which='both', bottom='on', top='off',
                labelbottom='on', left='off', right='off', labelleft='on')

bits = [1, 8, 16, 32, 64, 128, 256]

for rank, column in enumerate(bits):
  column_rec_name = str(column)
  line = plt.plot(data.number_of_training_samples, 
                  data[column_rec_name],
                  lw=2.5,
                  color=color_sequence[rank])

  y_pos = data[column_rec_name][-1]

  plt.text(10010, y_pos, column, fontsize=14, color=color_sequence[rank])



# Note that if the title is descriptive enough, it is unnecessary to include
# axis labels; they are self-evident, in this plot's case.
plt.title('Precision on CIFAR dataset, as a function of training set (1000-10000) '
          'and number of bits (1 to 256)\n', fontsize=18, ha='center')

# Finally, save the figure as a PNG.
# You can also save it as a PDF, JPEG, etc.
# Just change the file extension in this call.
plt.savefig('results.png', bbox_inches='tight')

plt.show()