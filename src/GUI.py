import dearpygui.dearpygui as dpg
from math import *
import serial

def placeholder_function():
    pass

t_digital_plot = 0

arduino = serial.Serial("COM3", 9600)

dpg.create_context()
dpg.create_viewport()
dpg.setup_dearpygui()

with dpg.window(label="Temporary Test Window"):
    # print("This works")
    """dpg.add_text("Hello World!")
    # when caling lambda functions we don't use the () becaue the lambda calls it inside it's code,
    # we just pass what function it has to call, it then calls the function otherwise it would
    # have been called straight away
    dpg.add_button(label="Use placeholder function",
                   callback=placeholder_function)
    dpg.add_input_text(label="string")
    dpg.add_slider_float(label="float")"""
    paused = False
    temp_info = []
    distance_info = []
    show_digital = [True]
    show_analog = [True]

    def change_val(arr, ind, val):
        arr[ind] = val

    with dpg.group(horizontal=True):
        dpg.add_checkbox(label="digital_0", callback=lambda s, a: change_val(show_digital, 0, a),
                         default_value=True)
        dpg.add_checkbox(label="digital_1", callback=lambda s, a: change_val(show_digital, 1, a),
                         default_value=False)
        dpg.add_checkbox(label="analog_0", callback=lambda s, a: change_val(show_analog, 0, a),
                         default_value=True)
        dpg.add_checkbox(label="analog_1", callback=lambda s, a: change_val(show_analog, 1, a),
                         default_value=False)

    with dpg.plot(tag="_demo_digital_plot", width=500):
        # TODO: better handling of show/hide (more consistency between checkboxes and legend)
        dpg.add_plot_axis(dpg.mvXAxis, label="x", tag="x_axis_digital")
        dpg.set_axis_limits(dpg.last_item(), -10, 0)
        with dpg.plot_axis(dpg.mvYAxis, label="y"):
            dpg.set_axis_limits(dpg.last_item(), -2, 1.5)
            dpg.add_digital_series(
                [], [], label="digital_0", tag="digital_0")
            dpg.add_digital_series(
                [], [], label="digital_1", tag="digital_1")
            dpg.add_line_series([], [], label="analog_0", tag="analog_0")
            dpg.add_line_series([], [], label="analog_1", tag="analog_1")

    def _update_plot():
        # print("what about this")
        
        
        
        global t_digital_plot
        if not paused:
            t_digital_plot += dpg.get_delta_time()
            dpg.set_axis_limits(
                'x_axis_digital', t_digital_plot - 10, t_digital_plot)
            if show_digital[0]:
                data_digital[0].append(
                    [t_digital_plot, 1 if sin(t_digital_plot) > 0.45 else 0])
                dpg.set_value("digital_0", [*zip(*data_digital[0])])
            if show_digital[1]:
                data_digital[1].append(
                    [t_digital_plot, 1 if sin(t_digital_plot) < 0.45 else 0])
                dpg.set_value("digital_1", [*zip(*data_digital[1])])
            if show_analog[0]:
                data_analog[0].append(
                    [t_digital_plot, sin(t_digital_plot)])
                dpg.set_value("analog_0", [*zip(*data_analog[0])])
            if show_analog[1]:
                data_analog[1].append(
                    [t_digital_plot, cos(t_digital_plot)])
                dpg.set_value("analog_1", [*zip(*data_analog[1])])

    with dpg.item_handler_registry(tag="__demo_digital_plot_ref"):
        print("Does this work?")
        dpg.add_item_visible_handler(callback=_update_plot)
    dpg.bind_item_handler_registry(
        "_demo_digital_plot", dpg.last_container())

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()
