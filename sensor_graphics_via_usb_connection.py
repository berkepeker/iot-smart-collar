import sys
import serial
from collections import deque
import numpy as np
import pyqtgraph as pg
from PyQt5.QtWidgets import QApplication
from pyqtgraph.Qt import QtCore

# Arduino ile seri bağlantı
ser = serial.Serial('COM3', 38400)  # COM3 Arduino'nun bağlı olduğu portu temsil eder,

# Veri depolama için deque kullanılacak
buffer_length = 100  # Grafikte gösterilecek maksimum veri sayısı
data_buffer = {
    'pulse': deque(maxlen=buffer_length),
    'accel_x': deque(maxlen=buffer_length),
    'accel_y': deque(maxlen=buffer_length),
    'accel_z': deque(maxlen=buffer_length),
    'gyro_x': deque(maxlen=buffer_length),
    'gyro_y': deque(maxlen=buffer_length),
    'gyro_z': deque(maxlen=buffer_length),
    'temp_c': deque(maxlen=buffer_length),
    'temp_f': deque(maxlen=buffer_length),
}

# PyQtGraph ile grafik oluşturma
app = QApplication([])  # QApplication'ı buradan doğrudan içe aktarın
win = pg.GraphicsLayoutWidget(show=True, title="Arduino Veri Görselleştirme")
win.resize(1000, 600)

# Akselometre Grafikleri
ax = win.addPlot(title="Akselometre Verileri")
ax.setXRange(0, buffer_length)
ax.setYRange(-2, 2)
curve_ax_x = ax.plot(pen='r')
curve_ax_y = ax.plot(pen='g')
curve_ax_z = ax.plot(pen='b')

# Gyrometre Grafikleri
gy = win.addPlot(title="Jiroskop Verileri")
gy.setXRange(0, buffer_length)
gy.setYRange(-250, 250)
curve_gy_x = gy.plot(pen='r')
curve_gy_y = gy.plot(pen='g')
curve_gy_z = gy.plot(pen='b')

# Sıcaklık Grafikleri
temp = win.addPlot(title="Sıcaklık Verileri")
temp.setXRange(0, buffer_length)
temp.setYRange(0, 50)
curve_temp_c = temp.plot(pen='r')
curve_temp_f = temp.plot(pen='g')

# Nabız Grafikleri
pulse = win.addPlot(title="Nabız Verisi")
pulse.setXRange(0, buffer_length)
pulse.setYRange(0, 1023)
curve_pulse = pulse.plot(pen='b')

# Veri alma ve güncelleme fonksiyonu
def update():
    while ser.inWaiting() > 0:
        try:
            line = ser.readline().decode().strip()
            if line.startswith('NABIZ:'):
                pulse_val = int(line.split(':')[1])
                data_buffer['pulse'].append(pulse_val)
            elif line.startswith('Akselo X:'):
                accel_x = float(line.split(':')[1])
                accel_y = float(ser.readline().decode().strip().split(':')[1])
                accel_z = float(ser.readline().decode().strip().split(':')[1])
                data_buffer['accel_x'].append(accel_x)
                data_buffer['accel_y'].append(accel_y)
                data_buffer['accel_z'].append(accel_z)
            elif line.startswith('Gyro X:'):
                gyro_x = float(line.split(':')[1])
                gyro_y = float(ser.readline().decode().strip().split(':')[1])
                gyro_z = float(ser.readline().decode().strip().split(':')[1])
                data_buffer['gyro_x'].append(gyro_x)
                data_buffer['gyro_y'].append(gyro_y)
                data_buffer['gyro_z'].append(gyro_z)
            elif line.startswith('Derece Celsius:'):
                temp_c = float(line.split(':')[1])
                temp_f = float(ser.readline().decode().strip().split(':')[1])
                data_buffer['temp_c'].append(temp_c)
                data_buffer['temp_f'].append(temp_f)
            elif line.startswith('GPS:'):
                temp_g = line.split(':')[1]
                print('GPS Verisi: ')
                print(temp_g)
        except ValueError:
            pass

    curve_ax_x.setData(np.array(data_buffer['accel_x']))
    curve_ax_y.setData(np.array(data_buffer['accel_y']))
    curve_ax_z.setData(np.array(data_buffer['accel_z']))

    curve_gy_x.setData(np.array(data_buffer['gyro_x']))
    curve_gy_y.setData(np.array(data_buffer['gyro_y']))
    curve_gy_z.setData(np.array(data_buffer['gyro_z']))

    curve_temp_c.setData(np.array(data_buffer['temp_c']))
    curve_temp_f.setData(np.array(data_buffer['temp_f']))

    curve_pulse.setData(np.array(data_buffer['pulse']))

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(50)

if __name__ == '__main__':
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QApplication.instance().exec_()
