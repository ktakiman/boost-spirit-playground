local function build()
  -- can I catch failure?
  vim.cmd({ cmd = '!', args = { './build.sh', '%' } })
end

local function run()
  local path = vim.api.nvim_buf_get_name(0)
  local last_slash = string.find(path, '/[^/]*$')
  local last_dot = string.find(path, '%.[^%.]*$')
  local arg = 'bin/' .. string.sub(path, last_slash + 1, last_dot - 1)
  vim.cmd({ cmd = 'FloatermNew', args = { arg } })
end

vim.keymap.set('n', '<leader>cc', function()
  build()
  run()
end, { desc = 'run build.sh' })
