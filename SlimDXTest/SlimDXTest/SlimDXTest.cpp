// SlimDXTest.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "Form1.h"

using namespace SlimDXTest;


//#using <SlimDX>

//#using <System.Windows.Forms.dll>

//using SlimDX.Direct3D9;
//#using <SlimDX.Windows>

using namespace SlimDX;
using namespace SlimDX::Direct2D;

public ref class CHoge {
public:
	// Direct2D用
	SlimDX::Windows::RenderForm ^RenFo;
	WindowRenderTarget ^wrt;
	SlimDX::Color4	col1;
	SlimDX::Color4	col2;
	SlimDX::Color4	col3;
	SlimDX::Color4	col4;
	SlimDX::Direct2D::Brush	^bru;
	SlimDX::Direct2D::Brush	^bru2;
	SlimDX::Direct2D::Brush	^bru3;
	System::Drawing::Rectangle	^rectan;
	System::Drawing::RectangleF	^rectanF;
	SlimDX::DirectWrite::TextFormat ^tf;
	System::String ^str;
	int RadiusX, RadiusY;

	void init2D( void )
	{	// 初期化処理つめあわせ
		RenFo = gcnew SlimDX::Windows::RenderForm("SlimDX - Direct2D Sample");

		Factory ^fc = gcnew Factory();
		SlimDX::DirectWrite::Factory ^DwFact = gcnew SlimDX::DirectWrite::Factory();

		WindowRenderTargetProperties ^pr = gcnew WindowRenderTargetProperties();
		pr->Handle = RenFo->Handle;
		pr->PixelSize = RenFo->ClientSize;

		WindowRenderTargetProperties wrtp;
		wrtp.Handle = RenFo->Handle;
		wrtp.PixelSize = RenFo->ClientSize;
		wrt = gcnew WindowRenderTarget( fc, wrtp );
		col1 = SlimDX::Color4(0.5f, 0.5f, 1.0f);
		col2 = SlimDX::Color4(0.5f, 0.5f, 0.7f);
		col3 = SlimDX::Color4(0.8f, 0.8f, 0.9f);
		col4 = SlimDX::Color4(1.0f, 0.5f, 0.3f);

		bru = gcnew SlimDX::Direct2D::SolidColorBrush( wrt, col1 );
		bru2 = gcnew SlimDX::Direct2D::SolidColorBrush( wrt, col2 );
		bru3 = gcnew SlimDX::Direct2D::SolidColorBrush( wrt, col3 );
		rectan = gcnew System::Drawing::Rectangle( 60,80,100,160 );
		rectanF = gcnew System::Drawing::RectangleF( 60.5F,80.5F,100.0F,160.0F );

		// フォント関連
		tf = gcnew SlimDX::DirectWrite::TextFormat( DwFact,
			"ＭＳ 明朝",
			SlimDX::DirectWrite::FontWeight::Normal,
			SlimDX::DirectWrite::FontStyle::Italic,
			SlimDX::DirectWrite::FontStretch::Normal,
			20.0f,
			"ja-jp" );

		// 動きに関する初期化
		RadiusX = 10;
		RadiusY = 50;

		// マウスに関する初期化
		RenFo->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &CHoge::Hoge_MouseDown);

		str = "餃子の王将";
	}
	void Run2D( void )
	{	// 実行
//		SlimDX::Windows::MainLoop ^ml = gcnew SlimDX::Windows::MainLoop( &obj->mlfunc );
		SlimDX::Windows::MainLoop ^ml = gcnew SlimDX::Windows::MainLoop( this, &CHoge::mlfunc2D );
		SlimDX::Windows::MessagePump::Run( RenFo, ml );	//  mp &mpump  = new SlimDX::Windows::MessagePump();
	}

	System::Void Hoge_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		str = "バーミヤン";
	}

//	static void mlfunc2D( void )
	void mlfunc2D( void )
	{	// メインループ？
		System::Drawing::RectangleF ff( 10.5F,10.5F,100.0F,60.0F );
		System::Drawing::RectangleF ff2( 60.5F,80.5F,100.0F,160.0F );

		wrt->BeginDraw();
		wrt->Clear( col4 );

		// ブラシをかける。
		wrt->DrawLine( bru ,0,0,100,50 );
		wrt->DrawRectangle( bru, ff );
		wrt->FillRectangle( bru2, ff2 );

		// 円
		System::Drawing::PointF pf( 180.0f, 115.0f );
		SlimDX::Direct2D::Ellipse eli;
		eli.Center = pf;
		eli.RadiusX = RadiusX;
		eli.RadiusY = RadiusY;
		wrt->DrawEllipse( bru3, eli );

		// テキスト文字列
		System::Drawing::RectangleF ff3( 45.0F,45.0F,110.0F,65.0F );
		wrt->DrawText( str, tf, ff3, bru );

		// マウス位置の取得など。

		wrt->EndDraw();

		// 動きを付けてみる。
		RadiusX += 10;
		if( RadiusX > 200 ) RadiusX = 10;

		RadiusY -= 5;
		if( RadiusY <= 0 ) RadiusY = 150;
	}
};

public ref class CPiyo {
public:
	SlimDX::Windows::RenderForm ^RenFo;
	SlimDX::Direct3D9::Device ^Dev9;
	SlimDX::Direct3D9::Direct3D ^d3D;
	SlimDX::Direct3D10::Device ^Dev10;
	SlimDX::Color4	col1;

	SlimDX::Direct3D9::Mesh ^meme;
	SlimDX::Direct3D9::Material ^mate;

	void init3D(void)
	{
		RenFo = gcnew SlimDX::Windows::RenderForm("SlimDX - Direct3D Sample");
		RenFo->ClientSize = Size(480,360);
		d3D = gcnew SlimDX::Direct3D9::Direct3D();
		SlimDX::Direct3D9::Direct3D9Exception excep;

		SlimDX::Direct3D9::PresentParameters ^PrePara;
		PrePara = gcnew SlimDX::Direct3D9::PresentParameters();
		PrePara->BackBufferFormat = SlimDX::Direct3D9::Format::X8R8G8B8;
		PrePara->BackBufferCount = 1;
		PrePara->BackBufferWidth = RenFo->ClientSize.Width;
		PrePara->BackBufferHeight = RenFo->ClientSize.Height;
		PrePara->Multisample = SlimDX::Direct3D9::MultisampleType::None;
		PrePara->SwapEffect = SlimDX::Direct3D9::SwapEffect::Discard;
		PrePara->EnableAutoDepthStencil = true;
		PrePara->AutoDepthStencilFormat = SlimDX::Direct3D9::Format::D16;
		PrePara->PresentFlags = SlimDX::Direct3D9::PresentFlags::DiscardDepthStencil;
		PrePara->PresentationInterval = SlimDX::Direct3D9::PresentInterval::Default;
		PrePara->Windowed = true;
		PrePara->DeviceWindowHandle = RenFo->Handle;

//		try {

		Dev9 = gcnew SlimDX::Direct3D9::Device( d3D, 0, SlimDX::Direct3D9::DeviceType::Hardware,
			RenFo->Handle, SlimDX::Direct3D9::CreateFlags::HardwareVertexProcessing, PrePara );

//		}
//		catch ( excep )
//		{
//			MessageBox::Show("title", "DirectX Initialization failed", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
//			RenFo->Close();
//		}

//		Dev10 = gcnew SlimDX::Direct3D10::Device( SlimDX::Direct3D10::DriverType::Hardware,
//									SlimDX::Direct3D10::DeviceCreationFlags::SingleThreaded );

		Dev9->SetRenderState( SlimDX::Direct3D9::RenderState::ZEnable, SlimDX::Direct3D9::ZBufferType::UseZBuffer );
		Dev9->SetRenderState( SlimDX::Direct3D9::RenderState::CullMode, SlimDX::Direct3D9::Cull::None );

		SlimDX::Direct3D9::Light ll;
		ll.Type = SlimDX::Direct3D9::LightType::Directional;
		ll.Diffuse = SlimDX::Color4( 0.9f, 0.9f, 0.9f );
		ll.Ambient = SlimDX::Color4( 0.4f, 0.4f, 0.4f );
		ll.Direction = SlimDX::Vector3( 0.0f, -1.0f, 0.0f );

		Dev9->SetLight( 0, ll );
		Dev9->SetRenderState( SlimDX::Direct3D9::RenderState::Lighting, true );
		Dev9->EnableLight(0, true);

//		SlimDX::Direct3D9::Mesh me2( Dev9, 0, 0 );
//		SlimDX::Direct3D9::Mesh me3;

		meme = SlimDX::Direct3D9::Mesh::CreateTeapot( Dev9 );

//		meme->
//		meme = gcnew SlimDX::Direct3D9::Mesh( Dev9, 0, 0 );
//		meme = SlimDX::Direct3D9::Mesh::FromFile( Dev9, "Test2.x", SlimDX::Direct3D9::MeshFlags::Use32Bit );

//		mate = meme->GetMaterials();
	
		col1 = SlimDX::Color4(0.1f, 0.2f, 0.3f);
	}
	float f1, f2, f3;
	float ry;
	Form1 ^fm;
	void Run3D( void )
	{	// 実行

		// フォーム生成してみる。
		fm = gcnew Form1();
		fm->Show();		// モードレスで。

		ry = 5000.0f;
		f1 = 0.2f;
		f2 = 0.2f;
		f3 = 0.2f;
		SlimDX::Windows::MainLoop ^ml = gcnew SlimDX::Windows::MainLoop( this, &CPiyo::mlfunc3D );
		SlimDX::Windows::MessagePump::Run( RenFo, ml );	//  mp &mpump  = new SlimDX::Windows::MessagePump();
	}

	void mlfunc3D( void )
	{	// メインループ？
		col1 = SlimDX::Color4( f1, f2, f3 );
		f1 += 0.001f;
		if( f1 > 0.5f ){
			f1 = 0.2f;
			f2 += 0.01f;
			if( f2 > 0.5f ) {
				f2 = 0.2f;
				f3 += 0.01f;
				if( f3 > 0.5f ) {
					f3 = 0.2f;
				}
			}
		}
		ry -= ( 10.0f + (float)( fm->GetVal(0) * 10 ) );


//		device.Clear(ClearFlags.Target|ClearFlags.ZBuffer, Color.Black, 1.0f, 0);
//		Dev9->Clear( SlimDX::Direct3D9::ClearFlags::Target, col1, 1.0f, 0 );
		Dev9->Clear( SlimDX::Direct3D9::ClearFlags::Target | SlimDX::Direct3D9::ClearFlags::ZBuffer, col1, 1.0f, 0 );

//		SlimDX::Matrix::LookAtRH *mlr = new 

		Dev9->BeginScene();

//		mat.RotationY( Window

//		device.SetTransform(TransformState.World, Matrix.RotationY(Environment.TickCount/1000.0f));
//		Dev9->SetTransform( SlimDX::Direct3D9::TransformState::World, SlimDX::Matrix::RotationY( 5000.0f / 1000.0f ) );
		Dev9->SetTransform( SlimDX::Direct3D9::TransformState::World, SlimDX::Matrix::RotationY( ry / 1000.0f ) );

		Dev9->SetTransform( SlimDX::Direct3D9::TransformState::View,
							SlimDX::Matrix::LookAtRH( SlimDX::Vector3(0.0f, 0.0f, -10.0f),
							SlimDX::Vector3(0.0f, 0.0f, 0.0f), SlimDX::Vector3(0.0f, 1.0f, 0.0f) ) );

//		device.SetTransform(TransformState.Projection,Matrix.PerspectiveFovRH(3.0f/4.0f, 1.0f, 1.0f, 100.0f)); 			
		Dev9->SetTransform( SlimDX::Direct3D9::TransformState::Projection,
							SlimDX::Matrix::PerspectiveFovRH(3.0f/4.0f, 1.0f, 1.0f, 100.0f) );

		meme->DrawSubset( 0 );
		Dev9->EndScene();
		Dev9->Present();
	}
};

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Direct3D
	CPiyo ^obj = gcnew CPiyo();
	obj->init3D();
	obj->Run3D();

	// Direct2D
//	CHoge ^obj = gcnew CHoge();
//	obj->init2D();
//	obj->Run2D();

/*

//	System::Windows::Forms::Dev  WindowsDevice dev;

//	SlimDX::Direct3D9::Device(  e ^dev;
//	SlimDX::Direct2D:://	System::Windows::Forms::Form ^fo = gcnew SlimDX::Windows::RenderForm("SlimDX - Direct2D Sample");
	SlimDX::Windows::RenderForm ^RenFo = gcnew SlimDX::Windows::RenderForm("SlimDX - Direct2D Sample");

	Factory ^fc = gcnew Factory();

	WindowRenderTargetProperties ^pr = gcnew WindowRenderTargetProperties();
	pr->Handle = RenFo->Handle;
	pr->PixelSize = RenFo->ClientSize;
//	SlimDX::Direct2D::WindowRenderTarget ^wrt = gcnew SlimDX::Direct2D::WindowRenderTarget( fc, pr );

	WindowRenderTargetProperties wrtp;
	wrtp.Handle = RenFo->Handle;
	wrtp.PixelSize = RenFo->ClientSize;
	WindowRenderTarget ^wrt = gcnew WindowRenderTarget( fc, wrtp );
*/

//	SlimDX::Windows::MainLoop ^ml = gcnew SlimDX::Windows::MainLoop( &mlfunc );

//	SlimDX::Windows::MainLoop ^ml = gcnew SlimDX::Windows::MainLoop( &obj->mlfunc );
//	SlimDX::Windows::MessagePump::Run( RenFo, ml );	//  mp &mpump  = new SlimDX::Windows::MessagePump();

/*
  target.BeginDraw();
              target.Clear(Color.Black);
              target.EndDraw();
*/

	//	SlimDX::Direct2D::De

//	SlimDX::Windows::MessagePump()->Run( RenFo, () => { } );

//	mp->Run( RenFo, () => { });

//		(form, () => { });



	// メイン ウィンドウを作成して、実行します
//	Application::Run(gcnew Form1());
	return 0;
}
