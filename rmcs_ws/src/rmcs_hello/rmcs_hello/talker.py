import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Talker(Node):
    def __init__(self):
        super().__init__('talker')
        self.pub = self.create_publisher(String, 'chatter', 10)
        self.i = 0
        self.create_timer(0.5, self.timer_callback)

    def timer_callback(self):
        msg = String()
        msg.data = f'Hello RMCS! 第 {self.i} 条消息'
        self.pub.publish(msg)
        self.get_logger().info(f'发布: "{msg.data}"')
        self.i += 1

def main(args=None):
    rclpy.init(args=args)
    node = Talker()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
