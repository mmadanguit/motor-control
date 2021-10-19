import tkinter as tk
import serial_cmd

class Serial_cmd_gui:

    def __init__(self):
        self.dev = serial_cmd.Serial_cmd()
        if self.dev.dev is not None:
            self.update_job = None
            self.root = tk.Tk()
            self.root.title('Serial_cmd GUI')
            self.root.protocol('WM_DELETE_WINDOW', self.shut_down)
            fm = tk.Frame(self.root)
            rMotor_slider = tk.Scale(self.root, from_ = 0, to = 250, orient = tk.HORIZONTAL, showvalue = tk.FALSE, command = self.set_rMotor_callback)
            rMotor_slider.set(50)
            rMotor_slider.pack(side = tk.TOP)
            lMotor_slider = tk.Scale(self.root, from_ = 0, to = 250, orient = tk.HORIZONTAL, showvalue = tk.FALSE, command = self.set_lMotor_callback)
            lMotor_slider.set(50)
            lMotor_slider.pack(side = tk.TOP)
            self.rSensor_status = tk.Label(self.root, text = 'rSensor is currently ????')
            self.rSensor_status.pack(side = tk.TOP)
            self.lSensor_status = tk.Label(self.root, text = 'lSensor is currently ????')
            self.lSensor_status.pack(side = tk.TOP)
            self.update_job = self.root.after(500, self.update_status)

    def set_rMotor_callback(self, value):
        self.dev.set_rMotor(int(value))

    def set_lMotor_callback(self, value):
        self.dev.set_lMotor(int(value))

    def update_status(self):
        self.rSensor_status.configure(text = 'rSensor is currently {:04d}'.format(self.dev.get_rSensor()))
        self.lSensor_status.configure(text = 'lSensor is currently {:04d}'.format(self.dev.get_lSensor()))
        self.update_job = self.root.after(50, self.update_status)

    def shut_down(self):
        if self.update_job is not None:
            self.root.after_cancel(self.update_job)
        self.root.destroy()

if __name__=='__main__':
    gui = Serial_cmd_gui()
    gui.root.mainloop()
