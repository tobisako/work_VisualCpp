#pragma once


namespace StlTest02 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		bool bMouseIcon;				// マウスアイコン追従フラグ
		bool bChangeEventStatus;		// イベントステータス変更処理
		int	iEventStatusID;				// イベントステータスＩＤ
		int iTimeCount;					// 
		int iExpireTime;				// 
		int iAppearCount;				// 出現カウント
		int	iLevel;						// レベル
		int iKao;						// 顔変化カウンタ
		bool bKaoFlg;					// 顔フラグ
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Label^  label1;
	public: 
	private: System::Windows::Forms::Button^  Replay;
	public: 
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
			GameInit();
		}

	private:
		void GameInit(void);				// ゲーム初期化処理
		void GameEventStatusChange(void);	// ゲームイベント状態変更
		void GamePrepare_Start(void);		// 準備：スタート画面
		void GamePrepare_Playing(void);		// 準備：ゲーム開始画面
		void GamePrepare_NextStage(void);	// 準備：ネクストステージ
		void GamePrepare_GameOver(void);	// 準備：ゲームオーバー画面
		void GameLoopProc(void);			// ゲームループ
		void GameLoop_Start(void);			// ゲームループ：スタート時
		void GameLoop_Playing(void);		// ゲームループ：プレイ中
		void GameEvent_startdisp( void );	// ゲームイベント
		Graphics ^g;
		cliext::list<CEnemy^>			lisidx;		// STL/list
		cliext::list<CEnemy^>::iterator	ite;		// STL/list iterator

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->Replay = (gcnew System::Windows::Forms::Button());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(142, 50);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(370, 108);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Replay
			// 
			this->Replay->Location = System::Drawing::Point(12, 12);
			this->Replay->Name = L"Replay";
			this->Replay->Size = System::Drawing::Size(138, 28);
			this->Replay->TabIndex = 1;
			this->Replay->Text = L"Replay";
			this->Replay->UseVisualStyleBackColor = true;
			this->Replay->Click += gcnew System::EventHandler(this, &Form1::Replay_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(550, 276);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(73, 42);
			this->pictureBox2->TabIndex = 2;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox2_MouseMove);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(229, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 12);
			this->label1->TabIndex = 3;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(686, 481);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->Replay);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::Form1_Paint);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseMove);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Replay_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Form1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private: System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
	private: System::Void pictureBox2_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
};
}

