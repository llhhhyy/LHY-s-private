import React from "react";
import {
    LockOutlined,
    UserOutlined,
} from '@ant-design/icons';
import { LoginFormPage, ProFormText } from '@ant-design/pro-components';
import useMessage from "antd/es/message/useMessage";
import { Link, useNavigate } from "react-router-dom";
import { BasicLayout } from "../components/layout";
import { login } from "../service/login";
import { handleBaseApiResponse } from "../utils/message";

const LoginPage = () => {
    const [messageApi, contextHolder] = useMessage();
    const navigate = useNavigate();

    const onSubmit = async (values) => {
        let email = values['username'];
        let password = values['password'];

        let res = await login(email, password);
        handleBaseApiResponse(res, messageApi, () => navigate("/"));
    };

    return (
        // <BasicLayout>
            <div style={{
                height: '100vh',
                backgroundImage: `url(${process.env.PUBLIC_URL}/background1.png)`,
                backgroundSize: 'cover'
            }}>
                {contextHolder}
                <div style={{
                    position: 'absolute',
                    top: '50%',
                    left: '35%',
                    transform: 'translate(-50%, -50%)',
                    width: 'min(100%, 1000px)'
                }}>
                    <LoginFormPage
                        logo={process.env.PUBLIC_URL + '/logo.webp'}
                        title="Book Store"
                        subTitle="电子书城"
                        onFinish={onSubmit}
                        style={{ width: '100%', maxWidth: 1000, padding: '10px' }}
                    >
                        <ProFormText
                            name="username"
                            fieldProps={{
                                size: 'large',
                                prefix: <UserOutlined className={'prefixIcon'} />,
                            }}
                            placeholder={'请输入用户名'}
                            rules={[
                                {
                                    required: true,
                                    message: '请输入用户名!',
                                },
                            ]}
                        />
                        <ProFormText.Password
                            name="password"
                            fieldProps={{
                                size: 'large',
                                prefix: <LockOutlined className={'prefixIcon'} />,
                            }}
                            placeholder={'请输入密码'}
                            rules={[
                                {
                                    required: true,
                                    message: '请输入密码！',
                                },
                            ]}
                        />
                        <div style={{ marginBlockEnd: 24 }}>
                            <Link to='/register'>新账号？前往注册</Link>
                            <a style={{ float: 'right' }} href="#/">忘记密码</a>
                        </div>
                    </LoginFormPage>
                </div>
            </div>
        // </BasicLayout>
    );
};

export default LoginPage;
