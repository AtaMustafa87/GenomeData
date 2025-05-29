import torch
import torch.nn as nn

self.rnn = nn.LSTM(input_size=num_features, hidden_size=hidden_dim,
                   num_layers=2, batch_first=True)
# Pseudo-code for a simple TCN block
self.tcn = nn.Sequential(
    nn.Conv1d(in_channels=num_features, out_channels=hidden_dim, kernel_size=3, padding=2, dilation=2),
    nn.ReLU(),
    nn.Conv1d(hidden_dim, hidden_dim, kernel_size=3, padding=4, dilation=4),
    nn.ReLU(),
    # ... (more layers/residuals)
)

self.transformer_layer = nn.TransformerEncoderLayer(d_model=hidden_dim, nhead=4)
self.transformer = nn.TransformerEncoder(self.transformer_layer, num_layers=2)

self.fc_reg = nn.Linear(hidden_dim, 1+4)  # predicts oo1 plus cs1-4

self.fc_bin = nn.Linear(hidden_dim, 20)   # outputs logits for v1..v20

out_seq, _ = self.rnn(x)                 # (batch, seq_len, hidden_dim)
h_last = out_seq[:, -1, :]              # last time-step
real_pred = self.fc_reg(h_last)         # shape (batch,5)
bin_logits = self.fc_bin(h_last)        # shape (batch,20)
bin_pred = torch.sigmoid(bin_logits)    # convert to probabilities if needed

loss_reg = nn.MSELoss()(pred_real, target_real)        # for oo1, cs1–4
loss_bin = nn.BCEWithLogitsLoss()(pred_logits, target_bin)  # for v1–v20
loss = loss_reg + loss_bin

loss = loss_reg + alpha * loss_bin



class ForecastModel(nn.Module):
    def __init__(self, input_dim, hidden_dim):
        super().__init__()
        # Example: a 2-layer LSTM
        self.rnn = nn.LSTM(input_dim, hidden_dim, num_layers=2, batch_first=True)
        # Regression head (oo1 + cs1–cs4 => 5 outputs)
        self.fc_reg = nn.Linear(hidden_dim, 5)
        # Classification head (v1–v20 => 20 outputs)
        self.fc_bin = nn.Linear(hidden_dim, 20)

    def forward(self, x):
        # x: (batch, seq_len, input_dim)
        out_seq, _ = self.rnn(x)
        h_last = out_seq[:, -1, :]        # use last time-step's hidden state
        real_out = self.fc_reg(h_last)    # linear outputs for real-valued targets
        bin_logits = self.fc_bin(h_last)  # raw logits for binary targets
        return real_out, bin_logits

model = ForecastModel(input_dim=44, hidden_dim=128)
real_pred, bin_logits = model(batch_inputs)
# Loss calculation:
loss_reg = nn.MSELoss()(real_pred, target_real) 
loss_bin = nn.BCEWithLogitsLoss()(bin_logits, target_binary)
loss = loss_reg + loss_bin
