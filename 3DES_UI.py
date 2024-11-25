import tkinter as tk
import subprocess

def run_3des(input_text):
    try:
        # Run the compiled C++ executable
        process = subprocess.Popen(
            ['./3DES'], 
            stdin=subprocess.PIPE, 
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE
        )
        
        # Send input to the C++ program
        input_data = f"e\nt\n{input_text}\n"
        stdout, stderr = process.communicate(input=input_data.encode())

        if stderr:
            raise Exception(stderr.decode())

        # Parse the output to extract the IV and encrypted text
        output_lines = stdout.decode().splitlines()
        iv_line = next(line for line in output_lines if line.startswith("IV (hex):"))
        encrypted_line = next(line for line in output_lines if line.startswith("Encrypted text (hex):"))

        iv = iv_line.split(": ")[1]
        encrypted_text = encrypted_line.split(": ")[1]

        return encrypted_text, iv

    except Exception as e:
        print(f"Error: {e}")
        return None, None

def show_encrypt_options():
    initial_frame.pack_forget()
    encrypt_frame.pack(pady=20)

def on_encrypt_text():
    encrypt_frame.pack_forget()
    text_input_frame.pack(pady=20)

def encrypt_text():
    input_text = text_input.get("1.0", tk.END).strip()
    encrypted_text, iv = run_3des(input_text)

    if encrypted_text and iv:
        text_input_frame.pack_forget()
        encrypted_output_frame.pack(pady=20)

        encrypted_text_output.delete("1.0", tk.END)
        encrypted_text_output.insert(tk.END, encrypted_text)

        iv_output.delete("1.0", tk.END)
        iv_output.insert(tk.END, iv)
    else:
        print("Failed to encrypt text.")

# Create the main window
root = tk.Tk()
root.title("3DES Encryption/Decryption")
root.geometry("400x300")

# Initial frame for mode selection
initial_frame = tk.Frame(root)
initial_frame.pack(pady=20)

prompt_label = tk.Label(initial_frame, text="What would you like to do?", font=("Arial", 14))
prompt_label.pack(pady=20)

encrypt_button = tk.Button(initial_frame, text="Encrypt", command=show_encrypt_options, width=15, height=2)
encrypt_button.pack(side=tk.LEFT, padx=10)

decrypt_button = tk.Button(initial_frame, text="Decrypt", command=lambda: print("Decrypt selected"), width=15, height=2)
decrypt_button.pack(side=tk.RIGHT, padx=10)

# Frame for encryption options
encrypt_frame = tk.Frame(root)

encrypt_prompt_label = tk.Label(encrypt_frame, text="Encrypt Text or File?", font=("Arial", 14))
encrypt_prompt_label.pack(pady=20)

text_button = tk.Button(encrypt_frame, text="Text", command=on_encrypt_text, width=15, height=2)
text_button.pack(side=tk.LEFT, padx=10)

file_button = tk.Button(encrypt_frame, text="File", command=lambda: print("Encrypt File selected"), width=15, height=2)
file_button.pack(side=tk.RIGHT, padx=10)

# Frame for text input
text_input_frame = tk.Frame(root)

text_input_label = tk.Label(text_input_frame, text="Enter text to encrypt:", font=("Arial", 12))
text_input_label.pack(pady=10)

text_input = tk.Text(text_input_frame, height=5, width=40)
text_input.pack(pady=10)

encrypt_text_button = tk.Button(text_input_frame, text="Encrypt", command=encrypt_text, width=15, height=2)
encrypt_text_button.pack(pady=10)

# Frame for displaying encrypted text and IV
encrypted_output_frame = tk.Frame(root)

encrypted_text_label = tk.Label(encrypted_output_frame, text="Encrypted Text:", font=("Arial", 12))
encrypted_text_label.pack(pady=5)

encrypted_text_output = tk.Text(encrypted_output_frame, height=5, width=40)
encrypted_text_output.pack(pady=5)

iv_label = tk.Label(encrypted_output_frame, text="IV:", font=("Arial", 12))
iv_label.pack(pady=5)

iv_output = tk.Text(encrypted_output_frame, height=1, width=40)
iv_output.pack(pady=5)

# Start the application
root.mainloop()