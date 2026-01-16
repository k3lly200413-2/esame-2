import dearpygui.dearpygui as dpg
import serial

# --- UTILITY FUNCTIONS ---
def _hsv_to_rgb(h, s, v):
    """
    Converts HSV color values to RGB. 
    (Note: This helper is defined but not currently used in the main logic below).
    """
    if s == 0.0: return (v, v, v)
    i = int(h*6.)
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (255*v, 255*t, 255*p)
    if i == 1: return (255*q, 255*v, 255*p)
    if i == 2: return (255*p, 255*v, 255*t)
    if i == 3: return (255*p, 255*q, 255*v)
    if i == 4: return (255*t, 255*p, 255*v)
    if i == 5: return (255*v, 255*p, 255*q)

# --- SERIAL COMMAND FUNCTIONS ---
def landCommand():
    """Sends the 'L' character to Arduino to trigger landing sequence."""
    if arduino:
        arduino.write(b'L')
        print("Sent L (Land)")

def takeOffCommand():
    """Sends the 'T' character to Arduino to trigger takeoff sequence."""
    if arduino:
        arduino.write(b'T')
        print("Sent T (Take Off)")

# --- SERIAL CONNECTION SETUP ---
try:
    # Attempt to connect to the specific COM port
    # Timeout is set to 0.1s to prevent the GUI from freezing if no data arrives
    arduino = serial.Serial("COM7", 9600, timeout=.1)
except Exception as e:
    # Handle connection failure gracefully so the GUI can still open
    print(f"Warning: Could not connect to Arduino: {e}")
    arduino = None

# --- DEARPYGUI SETUP ---
dpg.create_context()
dpg.create_viewport(title="Arduino Plotter", width=700, height=600)
dpg.setup_dearpygui()

# --- GLOBAL VARIABLES ---
t_digital_plot = 0  # X-axis counter (Time)
paused = False      # Plot pause state

# Visibility toggles for data series
show_analog = [True, True]

# Data buffers to store plot points: [[time, value], [time, value]...]
data_digital = [[], []]
data_analog = [[], []] 

def change_val(arr, ind, val):
    """Callback helper to update boolean lists from checkboxes."""
    arr[ind] = val

# --- FSM (FINITE STATE MACHINE) UI HELPERS ---
def create_fsm_row(prefix, labels):
    """
    Draws a visual row of 'LED' indicators.
    
    Args:
        prefix (str): Unique ID for this group (e.g., 'temp').
        labels (list): List of state names (e.g., ['IDLE', 'ACTIVE']).
    """
    with dpg.group(horizontal=True):
        for i, label in enumerate(labels):
            # Create a small canvas for each indicator
            with dpg.drawlist(width=100, height=120):
                # Draw the circle (LED). Tag format: "prefix_indicator_index"
                dpg.draw_circle((50, 50), 30, color=[50, 50, 50], fill=[50, 50, 50], 
                                tag=f"{prefix}_indicator_{i}")
                
                # Center the text below the circle
                text_offset = 50 - (len(label) * 3) 
                dpg.draw_text((text_offset, 95), label, size=15)

def set_fsm_state(prefix, active_index, total_steps):
    """
    Updates the colors of an FSM row. The active index turns Green, others turn Grey.
    """
    for i in range(total_steps):
        tag = f"{prefix}_indicator_{i}"
        
        if i == active_index:
            # Active State: Bright Green
            dpg.configure_item(tag, fill=[0, 255, 0], color=[0, 255, 0])
        else:
            # Inactive State: Dim Grey
            dpg.configure_item(tag, fill=[50, 50, 50], color=[50, 50, 50])

# --- MAIN GUI WINDOW ---
with dpg.window(label="Temporary Test Window", width=690, height=560):
    
    # 1. Define Custom Themes (Colors/Styles)
    # ---------------------------------------
    with dpg.theme(tag="theme_red"):
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 0, 0))          # Red Background
            dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, (255, 100, 100)) # Light Red (Clicked)
            dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, (200, 0, 0))    # Dark Red (Hover)
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 50)             # Rounded corners
            dpg.add_theme_style(dpg.mvStyleVar_FramePadding, 120, 50)         # Big Button Size

    with dpg.theme(tag="theme_blue"):
        with dpg.theme_component(dpg.mvButton):
            dpg.add_theme_color(dpg.mvThemeCol_Button, (0, 0, 255))          # Blue Background
            dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, (100, 100, 255)) # Light Blue
            dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, (0, 0, 200))    # Dark Blue
            dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 50) 
            dpg.add_theme_style(dpg.mvStyleVar_FramePadding, 120, 50) 

    # 2. Control Buttons
    # ------------------
    with dpg.group(horizontal=True):
        # Red Button -> Triggers Take Off
        dpg.add_button(label="Click Red", callback=takeOffCommand)
        dpg.bind_item_theme(dpg.last_item(), "theme_red")
        
        # Blue Button -> Triggers Land
        dpg.add_button(label="Click Blue", callback=landCommand)
        dpg.bind_item_theme(dpg.last_item(), "theme_blue")
    
    # 3. Checkboxes to toggle plot lines
    # ----------------------------------
    with dpg.group(horizontal=True):
        dpg.add_checkbox(label="TEMP (Analog 0)", default_value=show_analog[0], 
                         callback=lambda s, a: change_val(show_analog, 0, a))
        dpg.add_checkbox(label="DIST (Analog 1)", default_value=show_analog[1], 
                         callback=lambda s, a: change_val(show_analog, 1, a))

    # 4. Plot Setup
    # -------------
    with dpg.plot(tag="_demo_digital_plot", width=-1, height=-1):
        dpg.add_plot_axis(dpg.mvXAxis, label="Time (s)", tag="x_axis_digital")
        
        with dpg.plot_axis(dpg.mvYAxis, label="Value"):
            dpg.set_axis_limits(dpg.last_item(), 0, 150) # Fixed Y-scale (0 to 150)
            
            # Initialize empty series for Digital (unused currently) and Analog data
            dpg.add_digital_series([], [], label="digital_0", tag="digital_0_series")
            dpg.add_digital_series([], [], label="digital_1", tag="digital_1_series")
            dpg.add_line_series([], [], label="TEMP", tag="analog_0_series")
            dpg.add_line_series([], [], label="DIST", tag="analog_1_series")

    # 5. Main Update Loop (Called every frame)
    # ----------------------------------------
    def _update_plot():
        global t_digital_plot
        
        # --- A. READ SERIAL DATA ---
        if arduino and arduino.in_waiting:
            try:
                # Read, Decode bytes to string, Remove newlines
                line_bytes = arduino.readline()
                line_str = line_bytes.decode('utf-8').strip()
                
                # Split line by spaces. Expected format: "LABEL VALUE" (e.g., "TEMP 24.5")
                parts = line_str.split()
                
                print(parts) # Debug output to console
                                
                # Case 1: Data Packet (Label + Number)
                if len(parts) == 2:
                    label = parts[0]
                    try:
                        value = float(parts[1])
                        
                        if label == "TEMP":
                            # Append [Time, Value] to Temp buffer
                            data_analog[0].append([t_digital_plot, value])
                        elif label == "DIST":
                            # Append [Time, Value] to Distance buffer
                            data_analog[1].append([t_digital_plot, value])
                            
                    except ValueError:
                        pass # Second part wasn't a number, ignore.
                
                # Case 2: Status Code (Single Char or other message)
                else:
                    # Pass the message to the FSM logic handler
                    checkLetter(parts[0])

            except Exception as e:
                print(f"Serial Error: {e}")

        # --- B. UPDATE PLOT VISUALS ---
        if not paused:
            # Advance time
            t_digital_plot += dpg.get_delta_time()
            
            # Scroll X-Axis (show last 10 seconds)
            dpg.set_axis_limits("x_axis_digital", t_digital_plot - 10, t_digital_plot)

            # Update TEMP Series
            if show_analog[0] and len(data_analog[0]) > 0:
                # Sliding Window: Remove old data points if buffer > 600
                if len(data_analog[0]) > 600: data_analog[0].pop(0)
                # DPG requires list of tuples or separate x/y lists. zip(*data) unzips them.
                dpg.set_value("analog_0_series", list(zip(*data_analog[0])))

            # Update DIST Series
            if show_analog[1] and len(data_analog[1]) > 0:
                if len(data_analog[1]) > 500: data_analog[1].pop(0)
                dpg.set_value("analog_1_series", list(zip(*data_analog[1])))

    # Bind the update function to the item handler so it runs continuously
    with dpg.item_handler_registry(tag="__demo_digital_plot_ref"):
        dpg.add_item_visible_handler(callback=_update_plot)
    dpg.bind_item_handler_registry("_demo_digital_plot", "__demo_digital_plot_ref")
    
    # 6. FSM Visual Components
    # ------------------------
    
    # Row 1: Temperature Status
    dpg.add_text("Temperature Sequence", color=[255, 100, 100])
    temp_steps = ["NORMAL", "ALARM"]
    create_fsm_row("temp", temp_steps)
    
    dpg.add_separator()
    dpg.add_spacer(height=20)

    # Row 2: Distance/Flight Status
    dpg.add_text("Distance Sequence", color=[100, 100, 255])
    dist_steps = ["REST", "TAKING OFF", "OPERATING", "LANDING"]
    create_fsm_row("dist", dist_steps)
    
    dpg.add_separator()
    
# --- LOGIC HANDLER ---
def checkLetter(char):
    """
    Parses single characters from Arduino to update FSM UI states.
    Returns True if a valid char was found.
    """
    print(char)
    
    # Distance/Flight State Logic
    if (char == 'R'):
        set_fsm_state("dist", 0, 4) # Set 'REST' active
        return True
    elif char == 'T':
        set_fsm_state("dist", 1, 4) # Set 'TAKING OFF' active
        return True
    elif char == 'O':
        print(char)
        set_fsm_state("dist", 2, 4) # Set 'OPERATING' active
        return True
    elif char == 'L':
        set_fsm_state("dist", 3, 4) # Set 'LANDING' active
        return True
    
    # Temperature State Logic
    elif char == 'N':
        set_fsm_state("temp", 0, 2) # Set 'NORMAL' active
        return True
    elif char == 'A':
        set_fsm_state("temp", 1, 2) # Set 'ALARM' active
        return True
    
    return False

# --- INITIALIZATION ---
# Set initial states (Index 0 active for both)
set_fsm_state("temp", 0, len(temp_steps))
set_fsm_state("dist", 0, len(dist_steps))

# --- RUN DPG ---
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()