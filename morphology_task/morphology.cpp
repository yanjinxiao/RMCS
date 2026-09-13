
#include <iostream>
#include <vector>
#include <functional>

// 模块化形态学处理类：结构元素可替换
class Morphology {
private:
    std::vector<std::vector<int>> kernel;  // 结构元素（二值矩阵）
    int k_h, k_w;

public:
    // 构造时传入结构元素
    Morphology(const std::vector<std::vector<int>>& k) : kernel(k) {
        k_h = kernel.size();
        k_w = kernel[0].size();
    }

    // 设置新的结构元素（模块化：随时替换）
    void set_kernel(const std::vector<std::vector<int>>& k) {
        kernel = k;
        k_h = kernel.size();
        k_w = kernel[0].size();
    }

    // 膨胀操作：结构元素覆盖区域只要有1，中心就为1
    std::vector<std::vector<int>> dilate(const std::vector<std::vector<int>>& input) {
        int h = input.size();
        int w = input[0].size();
        std::vector<std::vector<int>> output(h, std::vector<int>(w, 0));
        int pad_h = k_h / 2;
        int pad_w = k_w / 2;

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int max_val = 0;
                for (int ki = 0; ki < k_h; ki++) {
                    for (int kj = 0; kj < k_w; kj++) {
                        int ni = i + ki - pad_h;
                        int nj = j + kj - pad_w;
                        if (ni >= 0 && ni < h && nj >= 0 && nj < w && kernel[ki][kj] == 1) {
                            max_val = std::max(max_val, input[ni][nj]);
                        }
                    }
                }
                output[i][j] = max_val;
            }
        }
        return output;
    }

    // 腐蚀操作：结构元素覆盖区域全为1，中心才为1
    std::vector<std::vector<int>> erode(const std::vector<std::vector<int>>& input) {
        int h = input.size();
        int w = input[0].size();
        std::vector<std::vector<int>> output(h, std::vector<int>(w, 0));
        int pad_h = k_h / 2;
        int pad_w = k_w / 2;

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int min_val = 1;
                for (int ki = 0; ki < k_h; ki++) {
                    for (int kj = 0; kj < k_w; kj++) {
                        int ni = i + ki - pad_h;
                        int nj = j + kj - pad_w;
                        if (kernel[ki][kj] == 1) {
                            if (ni < 0 || ni >= h || nj < 0 || nj >= w) {
                                min_val = 0;  // 边界外算背景
                            } else {
                                min_val = std::min(min_val, input[ni][nj]);
                            }
                        }
                    }
                }
                output[i][j] = min_val;
            }
        }
        return output;
    }

    // 打印矩阵测试
    void print_matrix(const std::vector<std::vector<int>>& mat) {
        for (auto& row : mat) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    // 12x12测试图
    std::vector<std::vector<int>> input = {
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,0,0,0,0,0},
        {0,0,0,0,1,0,1,0,0,0,0,0},
        {0,0,0,0,0,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,1,0,0,0},
        {0,0,0,0,0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0}
    };

    // 3x3矩形结构元素（dilation1效果：直角膨胀）
    std::vector<std::vector<int>> rect_kernel = {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    };

    // 3x3圆盘形结构元素（dilation2效果：圆角膨胀，四个角为0）
    std::vector<std::vector<int>> circle_kernel = {
        {0,1,0},
        {1,1,1},
        {0,1,0}
    };

    Morphology morph(rect_kernel);
    std::cout << "=== 原始图像 ===" << std::endl;
    morph.print_matrix(input);

    std::cout << "\n=== 矩形结构元素膨胀（dilation1）===" << std::endl;
    auto dilated1 = morph.dilate(input);
    morph.print_matrix(dilated1);

    std::cout << "\n=== 圆盘结构元素膨胀（dilation2，圆角）===" << std::endl;
    morph.set_kernel(circle_kernel);
    auto dilated2 = morph.dilate(input);
    morph.print_matrix(dilated2);

    std::cout << "\n=== 矩形结构元素腐蚀 ===" << std::endl;
    morph.set_kernel(rect_kernel);
    auto eroded = morph.erode(input);
    morph.print_matrix(eroded);

    return 0;
}
