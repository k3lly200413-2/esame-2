import dearpygui.dearpygui as dpg
import serial


def _hsv_to_rgb(h, s, v):
    if s == 0.0: return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (255*v, 255*t, 255*p)
    if i == 1: return (255*q, 255*v, 255*p)
    if i == 2: return (255*p, 255*v, 255*t)
    if i == 3: return (255*p, 255*q, 255*v)
    if i == 4: return (255*t, 255*p, 255*v)
    if i == 5: return (255*v, 255*p, 255*q)
    

# main
# --- 1. SETUP SERIAL ---
try:
    # Ensure this matches your actual Arduino COM port
    arduino = serial.Serial("COM3", 9600, timeout=.1)
except Exception as e:
    print(f"Warning: Could not connect to Arduino: {e}")
    arduino = None

dpg.create_context()
dpg.create_viewport(title="Arduino Plotter", width=700, height=600)
dpg.setup_dearpygui()

t_digital_plot = 0
paused = False

# SEPARATE STATE (Checkboxes) FROM DATA (Plotting)
show_digital = [True, True]
show_analog = [True, True]

# Store [x, y] coordinates here
data_digital = [[], []]
data_analog = [[], []]

def change_val(arr, ind, val):
    arr[ind] = val

with dpg.window(label="Temporary Test Window", width=690, height=560):
    
    # 1. Define the Red Theme
    with dpg.theme(tag="theme_red"):
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 0, 0))          # Red
            dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, (255, 100, 100)) # Light Red (Press)
            dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, (200, 0, 0))    # Darker Red (Hover)
            
            # Styling (Rounded and Large)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 50) 
            dpg.add_theme_style(dpg.mvStyleVar_FramePadding, 120, 50) 

    # 2. Define the Blue Theme
    with dpg.theme(tag="theme_blue"):
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(dpg.mvThemeCol_Button, (0, 0, 255))          # Blue
            dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, (100, 100, 255)) # Light Blue (Press)
            dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, (0, 0, 200))    # Darker Blue (Hover)
            
            # Styling (Same as Red)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 50) 
            dpg.add_theme_style(dpg.mvStyleVar_FramePadding, 120, 50) 

    # 3. Create the Buttons
    with dpg.group(horizontal=True):
        
        # Button 1: RED
        dpg.add_button(label="Click Red")
        dpg.bind_item_theme(dpg.last_item(), "theme_red")
        
        # Button 2: BLUE
        dpg.add_button(label="Click Blue")
        dpg.bind_item_theme(dpg.last_item(), "theme_blue")
    
    # Checkboxes
    with dpg.group(horizontal=True):
        dpg.add_checkbox(label="TEMP (Analog 0)", default_value=show_analog[0], 
                         callback=lambda s, a: change_val(show_analog, 0, a))
        dpg.add_checkbox(label="DIST (Analog 1)", default_value=show_analog[1], 
                         callback=lambda s, a: change_val(show_analog, 1, a))

    # Plot Configuration
    with dpg.plot(tag="_demo_digital_plot", width=-1, height=-1):
        dpg.add_plot_axis(dpg.mvXAxis, label="Time (s)", tag="x_axis_digital")
        
        with dpg.plot_axis(dpg.mvYAxis, label="Value"):
            # Set Y limits to handle the Arduino values (e.g. 0 to 200)
            dpg.set_axis_limits(dpg.last_item(), 0, 150)
            
            # --- IMPORTANT: The tags here MUST match the tags in _update_plot ---
            dpg.add_digital_series([], [], label="digital_0", tag="digital_0_series")
            dpg.add_digital_series([], [], label="digital_1", tag="digital_1_series")
            dpg.add_line_series([], [], label="TEMP", tag="analog_0_series")
            dpg.add_line_series([], [], label="DIST", tag="analog_1_series")

    def _update_plot():
        global t_digital_plot
        
        # --- 2. READ ARDUINO DATA ---
        if arduino and arduino.in_waiting:
            try:
                # Read line -> Decode -> Strip whitespace
                line_bytes = arduino.readline()
                line_str = line_bytes.decode('utf-8').strip()
                
                # Split "TEMP 100" into parts
                parts = line_str.split()
                
                # We need exactly 2 parts: [Label, Value]
                if len(parts) == 2:
                    label = parts[0]
                    try:
                        value = float(parts[1])
                        
                        # Route data to correct list based on label
                        if label == "TEMP":
                            # Add to Analog 0 list
                            data_analog[0].append([t_digital_plot, value])
                        elif label == "DIST":
                            # Add to Analog 1 list
                            data_analog[1].append([t_digital_plot, value])
                            
                    except ValueError:
                        pass # Value part wasn't a number
            except Exception as e:
                print(f"Serial Error: {e}")

        # --- 3. UPDATE PLOT ---
        if not paused:
            t_digital_plot += dpg.get_delta_time()
            
            # Scroll X-Axis
            dpg.set_axis_limits("x_axis_digital", t_digital_plot - 10, t_digital_plot)

            # --- Update TEMP (Analog 0) ---
            if show_analog[0] and len(data_analog[0]) > 0:
                # Limit buffer size (remove old points)
                if len(data_analog[0]) > 500: data_analog[0].pop(0)
                # Update the series with the new list
                dpg.set_value("analog_0_series", list(zip(*data_analog[0])))

            # --- Update DIST (Analog 1) ---
            if show_analog[1] and len(data_analog[1]) > 0:
                if len(data_analog[1]) > 500: data_analog[1].pop(0)
                dpg.set_value("analog_1_series", list(zip(*data_analog[1])))

    # Bind the update function
    with dpg.item_handler_registry(tag="__demo_digital_plot_ref"):
        dpg.add_item_visible_handler(callback=_update_plot)
    dpg.bind_item_handler_registry("_demo_digital_plot", "__demo_digital_plot_ref")

dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()