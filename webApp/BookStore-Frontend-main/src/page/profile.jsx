import { Button, Card, Space } from "antd";
import { PrivateLayout } from "../components/layout";
import UserProfile from "../components/user_profile";
import { useNavigate } from "react-router-dom";

export default function ProfilePage() {
    const navigate = useNavigate(); // 使用 useNavigate 钩子

    return (
        <PrivateLayout>
            <Card className="card-container">
                <UserProfile />
                <Space style={{ marginTop: 16 }}>
                    <Button type="primary" onClick={() => navigate("/api-docs")}>
                        后端 API 文档
                    </Button>
                    <Button type="primary" onClick={() => navigate("/course")}>
                        课程
                    </Button>
                </Space>
            </Card>
        </PrivateLayout>
    );
}
