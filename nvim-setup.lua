local function build()
  vim.cmd({ cmd = '!', args = { './build.sh' } })
  return vim.v.shell_error == 0
end

local function run()
  vim.cmd({ cmd = 'FloatermNew', args = { 'ninja-clang-debug/bin/test' } })
end

vim.keymap.set('n', '<leader>cc', function()
  if build() then
    run()
  end
end, { desc = 'run build.sh' })
