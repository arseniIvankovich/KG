import os
import cv2
import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk
import matplotlib.pyplot as plt
import numpy as np


def open_folder():
    folder_path = filedialog.askdirectory()
    if folder_path:
        image_paths = [os.path.join(folder_path, filename) for filename in os.listdir(folder_path) if
                       filename.lower().endswith(('.jpg', '.jpeg', '.png'))]
        return image_paths
    return []


def open_single_image(process_function):
    file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.jpg;*.jpeg;*.png")])
    if file_path:
        image = cv2.imread(file_path)
        if image is not None:
            original_image = image.copy()
            processed_image = process_function(image)
            show_images(original_image, processed_image, file_path)


def show_images(original_image, processed_images, titles):
    fig, axes = plt.subplots(1, len(processed_images) + 1, figsize=(12, 5))
    axes[0].axis('off')

    for i, (processed_image, title) in enumerate(zip(processed_images, titles)):
        axes[i + 1].set_title(title)
        axes[i + 1].imshow(cv2.cvtColor(processed_image, cv2.COLOR_BGR2RGB))
        axes[i + 1].axis('off')

    plt.show()


def blur_image(image):
    kernel_size = (5, 5)
    return cv2.blur(image, kernel_size)


def gaussian_blur(image):
    kernel_size = (5, 5)
    return cv2.GaussianBlur(image, kernel_size, 0)


def adaptive_threshold(image):
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    return cv2.adaptiveThreshold(gray_image, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)


def otsu_thresholding(image):
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, thresholded = cv2.threshold(gray_image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    return thresholded


def histogram_thresholding(image):
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Вычисляем гистограмму
    hist = cv2.calcHist([gray_image], [0], None, [256], [0, 256])

    # Нормализуем гистограмму
    hist /= hist.sum()

    # Вычисляем порог, максимизируя интерклассовую дисперсию
    best_threshold = 0
    best_variance = 0
    total_pixels = gray_image.size

    for threshold in range(256):
        background = hist[:threshold].sum()
        foreground = hist[threshold:].sum()

        if background == 0 or foreground == 0:
            continue  # Обработка случаев с нулевым знаменателем

        mean_background = np.sum(np.arange(threshold) * hist[:threshold]) / background
        mean_foreground = np.sum(np.arange(threshold, 256) * hist[threshold:]) / foreground

        variance_between_classes = background * foreground * (mean_background - mean_foreground) ** 2

        if variance_between_classes > best_variance:
            best_variance = variance_between_classes
            best_threshold = threshold

    _, thresholded = cv2.threshold(gray_image, best_threshold, 255, cv2.THRESH_BINARY)

    return thresholded


root = tk.Tk()
root.title("Image Processing")

original_image = None


def process_original_image():
    global original_image
    file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.jpg;*.jpeg;*.png")])
    if file_path:
        image = cv2.imread(file_path)
        if image is not None:
            original_image = image.copy()

def process_images(methods):
    global original_image
    if original_image is not None:
        processed_images = [original_image]
        titles = ["Original Image"]
        for method in methods:
            processed_image = method(original_image)
            processed_images.append(processed_image)
            titles.append(method.__name__.replace("_", " ").title())
        show_images(original_image, processed_images, titles)


button_original = tk.Button(root, text="Choose Original Image", command=process_original_image)
button_original.pack()

button_process = tk.Button(root, text="Process Images", command=lambda: process_images([blur_image, gaussian_blur]))
button_process.pack()

button_process2 = tk.Button(root, text="Process Images (2nd Method)", command=lambda: process_images(
    [adaptive_threshold, otsu_thresholding, histogram_thresholding]))
button_process2.pack()

root.mainloop()
