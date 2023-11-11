import cv2
import numpy as np
import matplotlib.pyplot as plt

# Загрузка изображения
image_path = "C:\\Users\\37544\\Python\\BSU\\PCG\\LAB3\\Screenshot 2023-11-06 112227.png"
image = cv2.imread(image_path, cv2.IMREAD_COLOR)

# Создание графика с 4x4 подграфиками
fig, axes = plt.subplots(3, 4, figsize=(9, 9))

# Отображение оригинального изображения
axes[0, 0].imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
axes[0, 0].set_title('Original Image')
axes[0, 0].axis("off")
axes[2, 3].axis("off")
# Функция добавления константы к каждому каналу каждого пикселя
def ModificationAddionOfConstant(image):
    image_with_constant = cv2.addWeighted(image, 1, np.zeros(image.shape, image.dtype), 0, constant)
    return image_with_constant

# Добавление константы
constant = 50
axes[0, 1].imshow(ModificationAddionOfConstant(image))
axes[0, 1].set_title('Add Constant')
axes[0, 1].axis("off")

# Функция умножения на константу
def ModificationMultConst(image):
    image_mult_const = (image.astype(np.float32) * constant).clip(0, 255).astype(np.uint8)
    return image_mult_const

# Умножение на константу
constant = 0.6
axes[0, 2].imshow(ModificationMultConst(image))
axes[0, 2].set_title('Multiply Constant')
axes[0, 2].axis("off")

# Функция инвертирования изображения
def Negative(image):
    negative_image = 255 - image
    return negative_image

# Негатив изображения
axes[0, 3].imshow(Negative(image))
axes[0, 3].set_title('Negative Image')
axes[0, 3].axis("off")

# Функция степенного преобразования
degree = 2
def DegreeModification(image):
    normalized_image = image.astype(np.float32) / 255.0
    degree_image = (np.power(normalized_image, degree) * 255).clip(0, 255).astype(np.uint8)
    return degree_image

# Степенное преобразование
axes[1, 0].imshow(DegreeModification(image))
axes[1, 0].set_title('Degree Image')
axes[1, 0].axis("off")

# Функция логарифмического преобразования
def LogModification(image):
    normalized_image = image.astype(np.float32) / 255.0
    sqrt_image = (np.sqrt(normalized_image) * 255).clip(0, 255).astype(np.uint8)
    return sqrt_image

# Логарифмическое преобразование
axes[1, 1].imshow(LogModification(image))
axes[1, 1].set_title('Log (sqrt) Image')
axes[1, 1].axis("off")

# Функция линейной коррекции контраста
def LinearContrast1(image):
    min_value = np.min(image)
    max_value = np.max(image)
    output_image = np.clip((255 / (max_value - min_value)) * (image - min_value), 0, 255).astype(np.uint8)
    return output_image

# Линейная коррекция контраста 1
axes[1, 2].imshow(LinearContrast1(image))
axes[1, 2].set_title('Linear contrast Image 1')
axes[1, 2].axis("off")

# Функция линейной коррекции контраста с заданными значениями
def LinearContrast2(image, new_min=0, new_max=400):
    min_value = np.min(image)
    max_value = np.max(image)
    output_image = np.clip(((image - min_value) / (max_value - min_value)) * (new_max - new_min) + new_min , 0, 255).astype(np.uint8)
    return output_image

# Линейная коррекция контраста 2
axes[1, 3].imshow(LinearContrast2(image))
axes[1, 3].set_title('Linear contrast Image 2')
axes[1, 3].axis("off")

# Функция бинаризации методом Бернсена
def bernsen_threshold(image, block_size, contrast_threshold):
    result = np.zeros_like(image, dtype=np.uint8)
    
    height, width = image.shape[:2]
    
    for x in range(0, height, block_size):
        for y in range(0, width, block_size):
            block = image[x:x+block_size, y:y+block_size]
            
            min_intensity = np.min(block)
            max_intensity = np.max(block)
            
            if max_intensity - min_intensity < contrast_threshold:
                result[x:x+block_size, y:y+block_size] = min_intensity
            else:
                result[x:x+block_size, y:y+block_size] = 255
    
    return result

# Функция бинаризации методом Ниблэка
def niblack_threshold(image, window_size, k):
    result = np.zeros_like(image, dtype=np.uint8)
    
    height, width = image.shape[:2]
    
    for x in range(window_size//2, height - window_size//2):
        for y in range(window_size//2, width - window_size//2):
            window = image[x - window_size//2:x + window_size//2 + 1, y - window_size//2:y + window_size//2 + 1]
            mean_intensity = np.mean(window)
            std_dev_intensity = np.std(window)
            threshold = mean_intensity + k * std_dev_intensity
            
            result[x, y] = np.where(image[x, y] > threshold, 255, 0)
    
    return result

# Загрузка изображения в оттенках серого
image_path = "C:\\Users\\37544\\Python\\BSU\\PCG\\LAB3\\Screenshot 2023-11-06 152852.png"
image = cv2.imread(image_path, cv2.COLOR_BGR2GRAY)

# Отображение результатов

axes[2, 0].imshow(bernsen_threshold(image, block_size=5, contrast_threshold=50), cmap='gray')
axes[2, 0].set_title("Bernsen")
axes[2, 0].axis("off")

axes[2, 1].imshow(niblack_threshold(image, window_size=15, k=-0.2), cmap='gray')
axes[2, 1].set_title("Niblack")
axes[2, 1].axis("off")
def adaptive_mean_threshold(image, block_size, constant):
    result = np.zeros_like(image, dtype=np.uint8)
    height, width = image.shape[:2]

    for x in range(0, height, block_size):
        for y in range(0, width, block_size):
            block = image[x:min(x+block_size, height), y:min(y+block_size, width)]
            threshold = np.mean(block) - constant
            result[x:min(x+block_size, height), y:min(y+block_size, width)] = np.where(block > threshold, 255, 0)

    return result
axes[2, 2].imshow(adaptive_mean_threshold(image, block_size=5, constant=0), cmap='gray')
axes[2, 2].set_title("Adaptation")
axes[2, 2].axis("off")

# Отображение результатов
plt.tight_layout()
plt.show()
